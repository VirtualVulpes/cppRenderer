#ifndef CPPRENDERER_FRAMEBUFFER_H
#define CPPRENDERER_FRAMEBUFFER_H


class Framebuffer {
public:
  Framebuffer(int width, int height);
  ~Framebuffer();

  void Bind() const;
  static void Unbind();

  void Resize(int width, int height);

  unsigned int GetColorAttachment() const;
  unsigned int GetDepthAttachment() const;

  int getWidth() const { return width_; }
  int getHeight() const { return height_; }

private:
  void Invalidate();

  unsigned int fbo_{0};
  unsigned int color_attachment_{0};
  unsigned int depth_attachment_{0};

  int width_;
  int height_;
};


#endif //CPPRENDERER_FRAMEBUFFER_H