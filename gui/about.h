#ifndef XSABOUT_H
#define XSABOUT_H

#include <QWidget>
#include <QLabel>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>

class about : public QWidget
{
        Q_OBJECT
public:
        QLabel *lb_logo;
        QLabel *lb_des;
        QLabel *label;
        explicit about(QWidget *parent = 0) :
                QWidget(parent)
        {
                if (objectName().isEmpty())
                        setObjectName(QStringLiteral("about"));
                resize(647, 393);
                QIcon icon;
                icon.addFile(QStringLiteral(":/xs/shield.png"), QSize(), QIcon::Normal, QIcon::Off);
                setWindowIcon(icon);
                lb_logo = new QLabel(this);
                lb_logo->setObjectName(QStringLiteral("lb_logo"));
                lb_logo->setGeometry(QRect(420, 10, 241, 271));
                lb_logo->setPixmap(QPixmap(QString::fromUtf8(":/resource/men.png")));
                lb_des = new QLabel(this);
                lb_des->setObjectName(QStringLiteral("lb_des"));
                lb_des->setGeometry(QRect(10, 20, 401, 271));
                lb_des->setTextFormat(Qt::AutoText);
                lb_des->setScaledContents(false);
                lb_des->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
                lb_des->setWordWrap(true);
                label = new QLabel(this);
                label->setObjectName(QStringLiteral("label"));
                label->setGeometry(QRect(10, 300, 621, 81));
                label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

                setWindowTitle(QApplication::translate("about", "About...", Q_NULLPTR));
                lb_logo->setText(QString());
                lb_des->setText(QApplication::translate("about", "<html><head/><body><p align=\"center\">"
                                                        "<span style=\" font-size:16pt; font-weight:600;\">"
                                                        "About XSoftware</span></p><p"
                                                        ">XSoftware is a Italian development team who believes in Open Source philosophy."
                                                        "</p><p align=\"center\"><span style=\" font-size:16pt; font-weight:600;\">"
                                                        "About Deb-Creator</span></p><p>Deb-Creator is a simple application designed to make the creation of debian packages easier. "
                                                        "</p><p>It aims to provide a straight forward graphical user interface and to automaticaly generate control "
                                                        "files and debian packages without the need for any command line knowledge.</p></body></html>", Q_NULLPTR));
                label->setText(QApplication::translate("about", "<html><head/><body><p>If you are a graphics designer or a software engineer, "
                                                       "feel free to get involved!</p><p>Check out the <a href=\"https://gitlab.com/xsoftware\">"
                                                       "<span style=\" text-decoration: underline; color:#007af4;\">gitlab page!</span></a></p>"
                                                       "<p>Or get in touch with us <span style=\" font-weight:600;\">git@xsoftware.eu</span></p></body></html>", Q_NULLPTR));
        }

        ~about()
        {
                delete lb_logo;
                delete lb_des;
                delete label;
        }
};



#endif // XSABOUT_H
