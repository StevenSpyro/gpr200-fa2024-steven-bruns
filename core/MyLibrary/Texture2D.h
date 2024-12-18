
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include"../ew/external/glad.h"
#include "../ew/external/stb_image.h"

namespace myLibrary
{
    class Texture2D
    {
    public:
        Texture2D(const char* filePath, int filterModeMin, int filterModeMag, int wrapModeS, int wrapModeT, int alpha);

        Texture2D();

        ~Texture2D();

        int getID() { return m_id; }

        void Bind(unsigned int slot = 0); //Bind to a specific texture unit

        void CreateDepthTexture(int width, int height);

    private:
        unsigned int m_id; //GL texture handle
        int m_width, m_height;
    };
}
