#include "v4l2grab.h"

int init_v4l2(void)
{
	// 1. Open device////////////////////////////////////
	if ((fd = open(FILE_VIDEO, O_RDWR)) == -1)
	{
		printf("Error opening V4L interface\n");
		return  FALSE;
	}
    /////////////////////////////////////////////////////

    // 2. Device setup///////////////////////////////////
	// Query device capabilities
	if (ioctl(fd, VIDIOC_QUERYCAP/*lab3.1_v4l2*/, &cap) == -1) {
		printf("Error opening device %s: unable to query device.\n",FILE_VIDEO);
		return  FALSE;
	}
	//else {
  //  printf("driver:\t\t%s\n"    , cap.driver);
  //  printf("card:\t\t%s\n"      , cap.card);
  //  printf("bus_info:\t%s\n"    , cap.bus_info);
  //  printf("version:\t%d\n"     , cap.version);
  //  printf("capabilities:\t%x\n", cap.capabilities);

  //  if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE)
  //  printf("Device %s: supports capture.\n",FILE_VIDEO);

	//	if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
	//		printf("Device %s: supports streaming.\n",FILE_VIDEO);
	//}

	// Print support format
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	//printf("Support format:\n");
	//while(ioctl(fd, VIDIOC_ENUM_FMT/*lab3.1_v4l2*/, &fmtdesc) != -1)
	//{
	//	printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
	//	fmtdesc.index++;
	//}

    // Set format
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV/*lab3.1_v4l2*/;
	fmt.fmt.pix.height = IMAGEHEIGHT;
	fmt.fmt.pix.width = IMAGEWIDTH;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    // Set the data format
	if(ioctl(fd, VIDIOC_S_FMT/*lab3.1_v4l2*/, &fmt) == -1)
	{
		printf("Unable to set format\n");
		return FALSE;
	}
    // Get the data format
	if(ioctl(fd, VIDIOC_G_FMT/*lab3.1_v4l2*/, &fmt) == -1)
	{
		printf("Unable to get format\n");
		return FALSE;
	}
	//{
  //  printf("fmt.type:\t\t%d\n", fmt.type);
  //  printf("pix.pixelformat:\t%c%c%c%c\n", fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF, (fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
  //  printf("pix.height:\t\t%d\n", fmt.fmt.pix.height);
  //  printf("pix.width:\t\t%d\n", fmt.fmt.pix.width);
  //  printf("pix.field:\t\t%d\n", fmt.fmt.pix.field);
	//}
	// Set fps
	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	setfps.parm.capture.timeperframe.numerator = 10;
	setfps.parm.capture.timeperframe.denominator = 10;
    /////////////////////////////////////////////////////

	//printf("init %s \t[OK]\n", FILE_VIDEO);

	return TRUE;
}

void v4l2_enqueue(void) {
	unsigned int i;
	// Queue
	for (i = 0; i < 16; i++) {
		buf.index = i;
        // Enqueue an empty (capturing) or filled (output) buffer in the driver's incoming queue
		//if (ioctl(fd, VIDIOC_QBUF/*lab3.1_v4l2*/, &buf))
    //        perror("Queue buffer error:");
		ioctl(fd, VIDIOC_QBUF/*lab3.1_v4l2*/, &buf);
	}
}

void v4l2_dequeue(void)
{
	if (ioctl(fd, VIDIOC_DQBUF/*lab3.1_v4l2*/, &buf)) {
    perror("Dequeue buffer error:");
  }
}

int v4l2_grab(void) {
	unsigned int n_buffers;

    // 3. Memory map/////////////////////////////////////
	// Request for 16 buffers
	req.count  = 16;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
    // Initiate memory mapping or user pointer I/O
	if (ioctl(fd, VIDIOC_REQBUFS/*lab3.1_v4l2*/, &req) == -1)
		perror("Request for buffers error:");

	// Memory allocate
	buffers = malloc(req.count * sizeof(*buffers));
	if (!buffers) {
		perror("Memory allocate error:");
		return FALSE;
	}

	for (n_buffers = 0; n_buffers < req.count; n_buffers++)
	{
		buf.type      = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory    = V4L2_MEMORY_MMAP;
		buf.index     = n_buffers;
		// Query buffers
		if (ioctl (fd, VIDIOC_QUERYBUF/*lab3.1_v4l2*/, &buf) == -1) {
			perror("query buffer error\n");
			return FALSE;
		}

		buffers[n_buffers].length = buf.length;
		// Memory map
		buffers[n_buffers].start = mmap(NULL, buf.length, PROT_READ |PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
		if (buffers[n_buffers].start == MAP_FAILED) {
			printf("buffer map error\n");
			return FALSE;
		}
	}

	// Queue
  v4l2_enqueue();
	//for (n_buffers = 0; n_buffers < req.count; n_buffers++) {
	//	buf.index = n_buffers;
  //      // Enqueue an empty (capturing) or filled (output) buffer in the driver's incoming queue
	//	if (ioctl(fd, VIDIOC_QBUF/*lab3.1_v4l2*/, &buf))
  //          perror("Queue buffer error:");
	//}
    /////////////////////////////////////////////////////
    // 4. Start capturing////////////////////////////////
    // Start streaming I/O
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(fd, VIDIOC_STREAMON/*lab3.1_v4l2*/, &type)) {
    perror("Start streaming error:");
    return FALSE;
  }
  //v4l2_dequeue();
  return TRUE;
}

int close_v4l2(void)
{
  // 9. Close device///////////////////////////////////
  if(fd != -1) {
      close(fd);
      return TRUE;
  }
  return FALSE;
  /////////////////////////////////////////////////////
}

void stop_streaming(void)
{
  unsigned int n_buffers;

  // 7. Stop streaming/////////////////////////////////
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(fd, VIDIOC_STREAMOFF/*lab3.1_v4l2*/, &type))
    perror("Stop streaming error:");
  /////////////////////////////////////////////////////

  // 8. Memory unmap///////////////////////////////////
  for (n_buffers = 0; n_buffers < req.count; n_buffers++)
    munmap(buffers[n_buffers].start, buffers[n_buffers].length);
  /////////////////////////////////////////////////////

  // 9. Close device///////////////////////////////////
  close_v4l2();
  /////////////////////////////////////////////////////
}
