/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2012-05-23
 */

//// Qt
//#include <QApplication>
//#include <QSplashScreen>
//#include <QtDebug>

//// Workbench
//#include "common.h"
//#include "qt_window_main.h"

 //test
#include "../geometry/polygon.h"
#include "../geometry/arrangement.h"


//#define _ELPP_THREAD_SAFE
//_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
//    LOG(INFO) << "===========================================================";

//    QApplication a(argc, argv);

//    // Qt5 provides a nice default GUI stylesheet called Fusion
//    //QApplication::setStyle(QStyleFactory::create("Fusion"));

//    // register any types that will pass through Qt's signal/slot mechanism
//    LOG(INFO) << "registering qt meta types";
//    qRegisterMetaType<QVector<DDAD::GL::Vertex>>("QVector<DDAD::GL::Vertex>");
//    qRegisterMetaType<QVector<QVector3D>>("QVector<QVector3D>");

//    LOG(INFO) << "displaying splash screen";
//    QPixmap splash_image(":/splash3.png");
//    QSplashScreen splash(splash_image);
//    splash.show();
//    splash.showMessage("Initializing...");
//    a.processEvents();

    std::cout << "Testing splay tree" << std::endl;
    DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
    int toinsert[] = {1, 2, 3, 4, 0};
    for(int ii = 0; ii < 5; ii++){
        spt.insert(toinsert[ii]);
    }

    std::cout << "\n Printing splay tree \n";
    spt.print();

    std::cout << "\nFinding element 5\n";
    spt.find(5);
    spt.print();

    std::cout << "\n Splitting the tree at 3\n";
    DDAD::SplayTree<int> lhs = DDAD::SplayTree<int>();
    lhs = spt.splitTree(3);
    std::cout << "\n Left-hand tree is \n";
    lhs.print();
    std::cout << "\nRight-hand tree is\n";
    spt.print();

    std::cout << "\n Merging lhs with spt\n";
    lhs.mergeTree(&spt);
    lhs.print();

    // Set up a sample arrangement for testing line intersections
    DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
    DDAD::Point_2r p, q;
    p = DDAD::Point_2r(DDAD::rational(DDAD::integer(-15), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(0), DDAD::integer(1)));
    q = DDAD::Point_2r(DDAD::rational(DDAD::integer(12), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(0), DDAD::integer(1)));
    sample_arrangement.AddSegment(p, q, true);
    p = DDAD::Point_2r(DDAD::rational(DDAD::integer(0), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(15), DDAD::integer(1)));
    q = DDAD::Point_2r(DDAD::rational(DDAD::integer(0), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(-12), DDAD::integer(1)));
    sample_arrangement.AddSegment(p, q, true);

    std::cout << DDAD::CountIntersections(sample_arrangement);




//    LOG(INFO) << "displaying main window";
//    MainWindow w;
//    w.showMaximized();
//    splash.finish(&w);

//    LOG(INFO) << "beginning main event loop";
//    return a.exec();
}
