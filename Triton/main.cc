#include "MainWindow.h"
#include "Triton.h"

int main(int argc, char *argv[])
{
    Triton a(argc, argv);
    
    QPixmap pixmap(":/images/triton.icon.png");
    QIcon icon(pixmap);
    QApplication::setWindowIcon(icon);
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
