#include "ScramblerCaesar.h"

#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <fstream>
#include <QDateTime>
#include <QMenu>
#include <QMenuBar>

const int widthApp = 450;
const int heighthApp = 450;
const int widthMoveBox = 60;
const int widthBtnEncr = 80;
const std::string filepath = "history.txt";

//English
const int alphabetLengthEn = 26;
const int beginLowRegisterEn = 65;
const int endLowRegisterEn = 90;
const int beginUpRegisterEn = 97;
const int endUpRegisterEn = 122;

ScramblerCaesar::ScramblerCaesar(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(heighthApp);
    setMinimumWidth(widthApp);

    pmnu = new QMenu("Menu");
    pmnu->addAction("History", this, &ScramblerCaesar::openHistory);
    mnuBar = new QMenuBar;
    mnuBar->addMenu(pmnu);

    enterLbl = new QLabel(tr("Enter text for encryption/decipher"));
    textInput = new QTextEdit;

    QLabel* actionLbl = new QLabel(tr("Action"));
    encryptR = new QRadioButton(tr("Encrypt"));
    encryptR->setChecked(true);
    decipherR = new QRadioButton(tr("Decipher"));
    QHBoxLayout* layoutsRadioB = new QHBoxLayout;
    layoutsRadioB->addWidget(encryptR);
    layoutsRadioB->addWidget(decipherR);
    layoutsRadioB->addStretch();

    QLabel* labelMove = new QLabel(tr("Move"));
    moveBox = new QComboBox;
    moveBox->setMaximumWidth(widthMoveBox);
    moveBox->setMinimumWidth(widthMoveBox);
    QStringList lst;
    for (int i = 1; i <= alphabetLengthEn; i++)
        lst << QString::number(i);
    moveBox->addItems(lst);

    textHistory = new QTextEdit;
    textHistory->setMinimumWidth(widthApp / 2);
    textHistory->setMinimumHeight(heighthApp / 2);
    textHistory->setReadOnly(true);

    encryptButton = new QPushButton(tr("Encrypt"));
    encryptButton->setMaximumWidth(widthBtnEncr);
    encryptButton->setMinimumWidth(widthBtnEncr);

    textOutput = new QTextEdit;
    textOutput->setReadOnly(true);

    connect(encryptButton, &QPushButton::released,this, &ScramblerCaesar::onEncrypted);
    connect(encryptR, &QRadioButton::toggled, this, &ScramblerCaesar::onClickedRadioB);

    QVBoxLayout* layoutWgt = new QVBoxLayout;
    layoutWgt->setContentsMargins(10, 10, 10, 10);
    layoutWgt->addWidget(enterLbl);
    layoutWgt->addWidget(textInput);
    layoutWgt->addWidget(actionLbl);
    layoutWgt->addLayout(layoutsRadioB);
    layoutWgt->addWidget(labelMove);
    layoutWgt->addWidget(moveBox);
    layoutWgt->addWidget(encryptButton);
    layoutWgt->addWidget(textOutput);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mnuBar);
    layout->addLayout(layoutWgt);

    setLayout(layout);
}

void ScramblerCaesar::onEncrypted()
{
    if (textInput->toPlainText().isEmpty())
    {
        textInput->setStyleSheet("QTextEdit { border: color: #853534 }");
        enterLbl->setStyleSheet("QLabel { color : red; }");
        return;
    }
    else
    {
        textInput->setStyleSheet("QTextEdit { border: color: #7A7A7A; }");
        enterLbl->setStyleSheet("QLabel { color : black; }");
    }
    QString changeText;
    std::string text = textInput->toPlainText().toStdString();
    auto te = textInput->toPlainText().toStdString();
    if (encryptR->isChecked())
        changeText = QString::fromStdString(caesarCipher(text, moveBox->currentText().toInt()));
    else
        changeText = QString::fromStdString(caesarCipher(text, (alphabetLengthEn - moveBox->currentText().toInt())));

    textOutput->setText(changeText);
    saveData();
}

void ScramblerCaesar::onClickedRadioB()
{
    if (encryptR->isChecked())
        encryptButton->setText("Encrypt");
    else
        encryptButton->setText("Decipher");
}

void ScramblerCaesar::openHistory()
{
    textHistory->hide();
    std::ifstream fRead;
    fRead.open(filepath);

    if (fRead.is_open())
    {
        char ch;
        std::string history;
        while (fRead.get(ch))
        {
            history += std::string{ch};
        }
        textHistory->setText(QString::fromStdString(history));
    }
    else
    {
        textHistory->setText("History empty");
    }
    textHistory->show();

    fRead.close();
}

std::string ScramblerCaesar::caesarCipher(std::string &s, int move)
{
    for (unsigned int i = 0; i < s.length(); i++)
    {
        if (s[i] >= beginLowRegisterEn && s[i] <= endLowRegisterEn)
        {
            if (s[i] + move > endLowRegisterEn)
                s[i] -= alphabetLengthEn;
            s[i] += move;
        }
        else if (s[i] >= beginUpRegisterEn && s[i] <= endUpRegisterEn)
        {
            if (s[i] + move > endUpRegisterEn)
                s[i] -= alphabetLengthEn;
            s[i] += move;
        }
    }
    return s;
}

void ScramblerCaesar::saveData()
{
    std::ofstream fWrite;
    fWrite.open(filepath, std::ofstream::app);
    if (fWrite.is_open())
    {
        auto time = QDateTime::currentDateTime().toString().toStdString();
        fWrite << time << "\nAction: ";
        if (encryptR->isChecked())
            fWrite << "Encrypt";
        else
            fWrite << "Decipher";
        fWrite << " on " << moveBox->currentText().toInt() << " move";
        fWrite << "\nText input:\n" << textInput->toPlainText().toStdString();
        fWrite << "\n\nText output:\n" << textOutput->toPlainText().toStdString() << "\n";
        fWrite << "--------------------------------------------------------\n\n";
    }
    fWrite.close();
}

