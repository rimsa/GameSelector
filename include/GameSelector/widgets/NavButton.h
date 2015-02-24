#ifndef NAVBUTTON_H
#define NAVBUTTON_H

#include <QPushButton>

class QGraphicsOpacityEffect;
class QPropertyAnimation;

class NavButton : public QPushButton {
    Q_OBJECT

public:
    NavButton(QWidget* parent = 0);
    virtual ~NavButton();

    virtual bool eventFilter(QObject* watched, QEvent* event);

protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

private:
    QGraphicsOpacityEffect* m_effect;
    QPropertyAnimation* m_animation;

private slots:
    void phaseIn();
    void phaseOut();

};

#endif // NAVBUTTON_H
