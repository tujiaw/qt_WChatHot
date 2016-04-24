#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>

class QLabel;
class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
    void setSelectedStyle(int index);
    int currentIndex() const;

signals:
    void sigButtonClicked(int oldIndex, int newIndex);

private slots:
    void slotTabClicked();

private:
    int m_currentIndex;
    QList<QLabel*> m_labelList;
};

#endif // TABWIDGET_H
