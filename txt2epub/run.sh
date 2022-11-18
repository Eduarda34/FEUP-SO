rm books/*.epub
rm ebooks.zip


gcc txt2epub.c -o txt2epub
./txt2epub books/*

