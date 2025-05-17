#include <gtest/gtest.h>
#include <text/textrender.hpp>
#include "config.hpp"

class TextRenderTest : public testing::Test {

protected:

    TextRenderTest() {

        mTextRender.loadFont(cad::config::arial_font_coordinates_file_path);

        cad::txt::Text T;

        T.text = "a";

        mTextRender.addText(T);

    }

    cad::txt::TextRender mTextRender;

};

// @brief
TEST_F(TextRenderTest, clearTexts) { 

    mTextRender.clearTexts();
    
    std::vector<cad::txt::Vertex> vertices{mTextRender.generateQuads(glm::vec3{})};
    
    EXPECT_TRUE(vertices.empty());

    std::vector<uint32_t> indices{mTextRender.generateIndices()};

    EXPECT_TRUE(indices.empty());

}

TEST_F(TextRenderTest, generateQuads) { 

    
    std::vector<cad::txt::Vertex> vertices{mTextRender.generateQuads(glm::vec3{})};
    
    EXPECT_EQ(vertices.size(), 4);

    cad::txt::Text T;

    T.text = "b";

    mTextRender.addText(T);

   vertices = mTextRender.generateQuads(glm::vec3{});
   
   EXPECT_EQ(vertices.size(), 8);

}

TEST_F(TextRenderTest, generateIndices) { 

    
    std::vector<cad::txt::Vertex> vertices{mTextRender.generateQuads(glm::vec3{})};
    std::vector<uint32_t> indices{mTextRender.generateIndices()};
    
    EXPECT_EQ(indices.size(), 6);

    cad::txt::Text T;

    T.text = "b";

    mTextRender.addText(T);

    vertices = mTextRender.generateQuads(glm::vec3{});
    indices = mTextRender.generateIndices();
    EXPECT_EQ(indices.size(), 12);

}

