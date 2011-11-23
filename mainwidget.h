#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTableView>

class QString;
class QTextEdit;
class QComboBox;
class QDomDocument;

class MainWidget : public QWidget
{

    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

    bool readXMLFile(QIODevice *device);
signals:

public slots:
    void klasseGewaehlt(QString sKlasse);
    void arbeitGewaehlt(QString sArbeit);

    void setText( QString text );

private:
    QTextEdit *textEdit;
    QString APP_PATH;
    QString KLASSEN_PATH;

    QComboBox *popupButtonKlassen;
    QComboBox *popupButtonArbeiten;

    QTableView *tableView;
    QDomDocument *domDocument;
};

#endif // MAINWIDGET_H
