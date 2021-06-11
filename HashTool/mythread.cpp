#include "mythread.h"
#include<openssl/evp.h>

mythread::mythread(MainWindow m)
{
    s = m.Filename;
    c = m.hashcombobox;
}

void mythread::run()
{
    std::string name3 = s;
    BIO  *b= BIO_new_file(name3.c_str(),"r");
    unsigned char digest[EVP_MAX_MD_SIZE];

    EVP_MD_CTX *ctx;
     ctx = EVP_MD_CTX_new();
    const  EVP_MD *md;
    unsigned int len;
     md = EVP_get_digestbyname(c->currentText().toLower().toStdString().c_str());
     if(md==NULL)qInfo()<<"error";

     EVP_DigestInit_ex(ctx,md,NULL);
    if(b != NULL){
        while (BIO_eof(b) != 1) {
            char m[2];
            size_t y;
            BIO_read_ex(b,(void *)m,2,&y);
           EVP_DigestUpdate(ctx,m,y);
        }
        std::string sp;
          EVP_DigestFinal_ex(ctx,digest,&len);
          for(unsigned int i=0;i<len;i++){
              char m[3];
              BIO_snprintf(m,3,"%02x",digest[i]);
              sp.append(m);
          }

          BIO_free(b);
emit hashready(QString(sp.c_str()));
}
}
