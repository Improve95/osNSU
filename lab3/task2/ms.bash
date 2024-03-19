rm createDirectory
rm listDirectory
rm removeDirectory
rm createFile
rm showFileContent
rm removeFile

gcc fileAction.c -o fa.out
ln fa.out createDirectory
ln fa.out listDirectory
ln fa.out removeDirectory
ln fa.out createFile
ln fa.out showFileContent
ln fa.out removeFile

./showFileContent ms.bash 