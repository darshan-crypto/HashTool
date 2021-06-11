#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("HashTool v1");
     init();
     addhash();
     signalattach();
     end();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    mainlayut = new QBoxLayout(tb,this);
     hashselector = new QBoxLayout(QBoxLayout::LeftToRight,this);
       type = new QBoxLayout(QBoxLayout::LeftToRight,this);
     hashalgo = new QLabel("Select Hash Algorithm ",this);
     hashcombobox = new QComboBox(this);
     text = new QRadioButton("Text",this);
     r1 = new QRadioButton("File",this);
     typelabel = new QLabel("Input Type",this);
     sublayout = new QBoxLayout(QBoxLayout::LeftToRight,this);
     Browse = new QPushButton("Browse",this);
     textbrowser = new QTextBrowser(this);
     name = new QLabel("Select File ",this);
      browselayout = new QBoxLayout(QBoxLayout::LeftToRight);

      buttonlayout = new QBoxLayout(QBoxLayout::LeftToRight);
      edit = new QLineEdit(this);
      calculate = new QPushButton("Calculate",this);
      diag = new QFileDialog(this,"select File For Hash");
      textbrowser->setTextInteractionFlags(Qt::TextInteractionFlags::enum_type::TextEditable);
      text->setChecked(true);
      edit->setPlaceholderText("Hash of Text");

}

void MainWindow::end()
{

    hashselector->addWidget(hashalgo);
    hashselector->addWidget(hashcombobox);

    type->addWidget(typelabel);
     type->addWidget(text);
      type->addWidget(r1);

sublayout->addWidget(textbrowser);

browselayout->addWidget(name);
browselayout->addWidget(Browse);
name->hide();
Browse->hide();
buttonlayout->addWidget(edit);
buttonlayout->addWidget(calculate);
edit->setReadOnly(1);

    mainlayut->addLayout(hashselector);
    mainlayut->addLayout(type);
    mainlayut->addLayout(sublayout);
    mainlayut->addLayout(buttonlayout);
    ui->centralwidget->setLayout(mainlayut);
}

void MainWindow::addhash()
{
    char hasharray[20][11]={"blake2b512","blake2s256","gost","md4","md5","rmd160","sha1","sha224","sha256","sha3-224","sha3-256","sha3-384","sha3-512","sha384","sha512","sha512-224","sha512-256","shake128","shaake256","sm3"};

   QList<QString> m;

    for(int i=0;i<20;i++){


    m << hasharray[i];
   }


  hashlist = new QStringList(m);
  hashcombobox->insertItems(0,*hashlist);
}

void MainWindow::signalattach()
{
    connect(r1,SIGNAL(toggled(bool)),this,SLOT(checked(bool)));
    connect(text,SIGNAL(toggled(bool)),this,SLOT(checked(bool)));
    connect(calculate,SIGNAL(clicked()),this,SLOT(calculateclicked()));
     connect(Browse,SIGNAL(clicked()),this,SLOT(browseclicked()));
    connect(textbrowser,SIGNAL(textChanged()),this,SLOT(calculateclicked()));
    connect(hashcombobox,SIGNAL(currentIndexChanged(int)),this,SLOT(combochanged(int)));
    connect(diag,SIGNAL(fileSelected(QString)),this,SLOT(itemselected(QString)));
    connect(th,SIGNAL(hashready(QString)),this,SLOT(setter(QString)));

}

void MainWindow::thread2(QString s)
{
    std::string name3 = s.toStdString();
    BIO  *b= BIO_new_file(name3.c_str(),"r");
    unsigned char digest[EVP_MAX_MD_SIZE];

    EVP_MD_CTX *ctx;
     ctx = EVP_MD_CTX_new();
    const  EVP_MD *md;
    unsigned int len;
     md = EVP_get_digestbyname(hashcombobox->currentText().toLower().toStdString().c_str());
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
          this->setter(sp.c_str());
          BIO_free(b);

}

}

void MainWindow::setter(QString s)
{
    edit->setText(s);
}


void MainWindow::checked(bool x )
{
    if(r1->isChecked()){
         edit->setPlaceholderText("Hash of File");
        sublayout->removeWidget(textbrowser);
        textbrowser->hide();
        sublayout->addWidget(name);
        sublayout->addWidget(Browse);
        name->show();
        Browse->show();
    }
    if(text->isChecked()){
        sublayout->removeWidget(name);
        sublayout->removeWidget(Browse);
        name->hide();
        Browse->hide();
        textbrowser->show();
         edit->setPlaceholderText("Hash of Text");
        sublayout->addWidget(textbrowser);
    }

}

void MainWindow::calculateclicked()
{
    edit->setPlaceholderText("calculating...");
 if(text->isChecked()){
EVP_MD_CTX *ctx;
 ctx = EVP_MD_CTX_new();
const  EVP_MD *md;
unsigned int len;
 md = EVP_get_digestbyname(hashcombobox->currentText().toLower().toStdString().c_str());
 if(md==NULL)qInfo()<<"error";
unsigned char digest[EVP_MAX_MD_SIZE];
 EVP_DigestInit_ex(ctx,md
                   ,NULL);
 std::string s9 = textbrowser->toPlainText().toStdString();
 EVP_DigestUpdate(ctx,s9.c_str(),s9.length());
 EVP_DigestFinal_ex(ctx,digest,&len);

  std::string s;
  for(unsigned int i=0;i<len;i++){
      char m[3];
      BIO_snprintf(m,3,"%02x",digest[i]);
      s.append(m);
  }


 edit->setText(QString(s.c_str()));
 }
 if(r1->isChecked()){
     itemselected(diag->selectedFiles().at(0));
 }
}

void MainWindow::combochanged(int)
{
    calculateclicked();
}

void MainWindow::browseclicked()
{
    diag->exec();
}

void MainWindow::itemselected(QString s)
{

    Filename = s.toStdString();

}
