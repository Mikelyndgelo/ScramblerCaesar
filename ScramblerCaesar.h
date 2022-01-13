#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <string>

class QTextEdit;
class QLabel;
class QPushButton;
class QRadioButton;
class QComboBox;
class QMenuBar;
class QMenu;

class ScramblerCaesar : public QWidget
{
    Q_OBJECT
public:
    explicit ScramblerCaesar(QWidget *parent = Q_NULLPTR);
private slots:
    void onEncrypted();
    void onClickedRadioB();
    void openHistory();
private:
    std::string caesarCipher(std::string &s, int move);
    QMenuBar* mnuBar;
    QMenu* pmnu;
    QLabel* enterLbl;
    QTextEdit* textInput;
    QTextEdit* textOutput;
    QTextEdit* textHistory;
    QPushButton* encryptButton;
    QComboBox* moveBox;
    QRadioButton* encryptR;
    QRadioButton* decipherR;
    void saveHistory();
};
#endif // WIDGET_H
