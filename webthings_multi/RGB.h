#ifndef __RGB_H
#define __RGB_H


class RGB
{
    private: 
    int _red, _green, _blue;

    public:
    RGB(int red, int green, int blue)
    {
      _red = red;
      _green = green;
      _blue = blue;
    }
    void Red();
    void Green();
    void Blue();
    void Cyan();
    void Purple();
    void Yellow();
    void White();
};




#endif
