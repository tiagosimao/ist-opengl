namespace window {
  bool init();
  bool shutdown();
  bool shouldClose();
  void getSize(int *width,int *height);
  void swapBuffers();
  void setTitle(const char *title);
}
