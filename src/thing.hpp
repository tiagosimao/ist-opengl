class Thing {
  public:
    Thing(char const * fileName);
    bool init();
    bool shutdown();
    void draw();
  private:
    char const * fileName;
};