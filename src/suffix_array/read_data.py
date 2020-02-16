import xml.sax

class WikiXmlHandler(xml.sax.handler.ContentHandler):
    """Content handler for Wiki XML data using SAX"""
    def __init__(self):
        xml.sax.handler.ContentHandler.__init__(self)
        self._buffer = None
        self._values = {}
        self._current_tag = None
        self._pages = []

    def characters(self, content):
        """Characters between opening and closing tags"""
        if self._current_tag:
            self._buffer.append(content)

    def startElement(self, name, attrs):
        """Opening tag of element"""
        if name in ('title', 'text'):
            self._current_tag = name
            self._buffer = []

    def endElement(self, name):
        """Closing tag of element"""
        if name == self._current_tag:
            self._values[name] = ' '.join(self._buffer)

        if name == 'page':
            self._pages.append((self._values['title'], self._values['text']))


import subprocess

data_path = '/home/simon/data/in/enwiki-20191201-pages-articles1.xml-p10p30302.bz2'

handler = WikiXmlHandler()

parser = xml.sax.make_parser()
parser.setContentHandler(handler)

# Iteratively process file
for line in subprocess.Popen(['bzcat'], 
                              stdin = open(data_path), 
                              stdout = subprocess.PIPE).stdout:
    parser.feed(line)

    # # Stop when 50 articles have been found
    # if len(handler._pages) == 200:
    #     break

# print(handler._pages[1])
# print(len(handler._pages[1]))
# print(handler._pages[1][1])

import mwparserfromhell

my_string = ""
for page in handler._pages:
    wiki = mwparserfromhell.parse(page[1])
    text = wiki.strip_code().strip()
    my_string += text

text_file_whole_string = open("/home/simon/data/out/string_200_articles.txt", "w")
text_file_whole_string.write(my_string)
text_file_whole_string.close()



    # print(page[0])


# whole_string = ""
# article_starts = []
# total_length = 0
# for page in handler._pages:
#     wiki = mwparserfromhell.parse(page[1])
#     text = wiki.strip_code().strip()
#     article_starts.append(len(whole_string))
#     whole_string += text
# article_starts.append(len(whole_string))

# text_file_whole_string = open("/home/simon/data/out/whole_string_medium_size.txt", "w")
# text_file_whole_string.write(whole_string)
# text_file_whole_string.close()

# text_file_article_starts = open("/home/simon/data/out/article_starts_medium_size.txt", "w")
# text_file_article_starts.write(str(len(article_starts)) + '\n')
# for start in article_starts:
#     text_file_article_starts.write(str(start) + '\n')
# text_file_article_starts.close()


# wiki = mwparserfromhell.parse(handler._pages[1][1])
# text = wiki.strip_code().strip()
# print(len(text))
# print(text[0:1000])
# for i in range(550, 560):
#     print(ord(text[i]), text[i])
# print(text[0:550])
