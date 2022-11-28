rm *.epub
rm ebooks.zip
cp books/* .


gcc txt2epub.c -o txt2epub

command="./txt2epub *.txt"

echo $command
echo 

$command
