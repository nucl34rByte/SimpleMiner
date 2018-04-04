For unzipping, add "unzip.cpp" to your project. Then, for example,
  #include "unzip.h"
  //
  HZIP hz = OpenZip("c:\\stuff.zip",0);
  ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems=ze.index;
  for (int i=0; i<numitems; i++)
  { GetZipItem(hz,i,&ze);
    UnzipItem(hz,i,ze.name);
  }
  CloseZip(hz);


For zipping, add "zip.cpp" to your project. (You can add just one of
zip/unzip, or both; they function independently and also co-exist.)
  #include "zip.h"
  //
  HZIP hz = CreateZip("c:\\simple1.zip",0);
  ZipAdd(hz,"znsimple.bmp", "c:\\simple.bmp");
  ZipAdd(hz,"znsimple.txt", "c:\\simple.txt");
  CloseZip(hz);

