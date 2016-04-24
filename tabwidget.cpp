#include "tabwidget.h"
#include <QtWidgets>

TabWidget::TabWidget(QWidget *parent) : QWidget(parent)
{
    auto createTab = [this](int index, const QString &text) -> QWidget* {
        QWidget *widget = new QWidget(this);
        QPushButton *button = new QPushButton(text, this);
        button->setProperty("index", index);
        button->setStyleSheet("background:transparent;");
        connect(button, SIGNAL(clicked()), this, SLOT(slotTabClicked()));
        QLabel *label = new QLabel(this);
        label->setFixedHeight(5);
        m_labelList.append(label);
        QVBoxLayout *vlayout = new QVBoxLayout();
        vlayout->setContentsMargins(0, 0, 0, 0);
        vlayout->setSpacing(0);
        vlayout->addWidget(button);;
        vlayout->addWidget(label);
        widget->setLayout(vlayout);
        return widget;
    };

    QHBoxLayout *mlayout = new QHBoxLayout();
    mlayout->setContentsMargins(0, 0, 0, 0);
    mlayout->setSpacing(0);
    mlayout->addWidget(createTab(0, tr("list")));
    mlayout->addWidget(createTab(1, tr("title")));
    mlayout->addWidget(createTab(2, tr("content")));
    mlayout->addStretch();
    this->setLayout(mlayout);

    setSelectedStyle(0);
    this->setStyleSheet("QWidget{font-size:14px; font-weight:bold; color:rgb(75, 75, 75); }");
}

void TabWidget::setSelectedStyle(int index)
{
    m_currentIndex = index;
    for (int i=0; i<m_labelList.count(); i++) {
        if (i == index) {
            m_labelList[i]->setStyleSheet("background:#ff0000;");
        } else {
            m_labelList[i]->setStyleSheet("background:transparent;");
        }
    }
}

void TabWidget::slotTabClicked()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
    if (button) {
        int oldIndex = m_currentIndex;
        int newIndex = button->property("index").toInt();
        setSelectedStyle(newIndex);
        emit sigButtonClicked(oldIndex, newIndex);
    }
}

int TabWidget::currentIndex() const
{
    return m_currentIndex;
}
