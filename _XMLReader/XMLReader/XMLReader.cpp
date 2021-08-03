//------------------------------------
// Jacob Zackaria (2021), Professor: Ed Keenan
//------------------------------------

#ifndef XML_READER_H
#define XML_READER_H

// No stl version
#define PUGIXML_NO_STL

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include "File.h"
#include "pugixml.hpp"

using namespace pugi;
using namespace Azul;

// custom font structure //
struct custom_font
{
    int key = 0;
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        Trace::out("\n\nBuild Complete!\n\n");
        printf("\n\n Build Complete!\n\n");
        return 0;
    }

    // File data.
    File::Handle fh = nullptr;
    File::Error status = File::Error::UNDEFINED;
    custom_font cf = { 0 };

    // Open file handle.
    status = File::Open(fh, argv[argc - 1], File::Mode::WRITE);
    assert(status == File::Error::SUCCESS);

    // seek to beginning.
    status = File::Seek(fh, File::Location::BEGIN, 0);
    assert(status == File::Error::SUCCESS);

    // read a document.
    xml_document doc;
    xml_parse_result result = doc.load_file(argv[argc - 2]);
    assert(result.status == xml_parse_status::status_ok);

    // traverse the DOM tree.
    // get first child
    xml_node tools = doc.child("fontMetrics");
    
    // loop through all character keys
    for (xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
    {
        printf("%s", tool.name());

        // loop through the attribute values.
        for (xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute())
        {
            printf(" %s = %s", attr.name(), attr.value());
            sscanf_s(attr.value(), "%d", &cf.key);
        }

        printf("\n");

        // loop through all child nodes. x, y ,width, height.
        for (xml_node tool2 = tool.first_child(); tool2; tool2 = tool2.next_sibling())
        {
            printf("\n");
            printf("   %s = %d", tool2.name(), tool2.text().as_int());
            printf("\n");

            if (*tool2.name() == 'x')
            {
                cf.x = tool2.text().as_float();
            }
            else if (*tool2.name() == 'y')
            {
                cf.y = tool2.text().as_float();
            }
            else if (*tool2.name() == 'w')
            {
                cf.width = tool2.text().as_float();
            }
            else if (*tool2.name() == 'h')
            {
                cf.height = tool2.text().as_float();
            }
        }

        status = File::Write(fh, &cf, sizeof(custom_font));
        assert(status == File::Error::SUCCESS);

        printf("\n");
    }

    // close file
    status = File::Close(fh);
    assert(status == File::Error::SUCCESS);

    // successfull
    return 0;
}

#endif XML_READER_H

