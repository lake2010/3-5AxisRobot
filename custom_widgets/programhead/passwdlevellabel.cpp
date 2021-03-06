#include <QEvent>
#include "passwdlevellabel.h"
#include "icparameterssave.h"


PasswdLevelLabel::PasswdLevelLabel(QWidget * parent)
    : QLabel(parent)
{
    passwordDialog_ = new PasswordDialog(this);
    connect(passwordDialog_,
            SIGNAL(LevelChanged(int)),
            this,
            SLOT(PasswdLevelChenged(int)));
    PasswdLevelChenged(ICParametersSave::MachineOperator);
}

PasswdLevelLabel::~PasswdLevelLabel()
{
    delete passwordDialog_;
}

static QString oldStyle;
void PasswdLevelLabel::mousePressEvent(QMouseEvent *ev)
{
    oldStyle = this->styleSheet();
    this->setStyleSheet("background-color:blue;color:white;");
}

void PasswdLevelLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    passwordDialog_->show();
    QLabel::mouseReleaseEvent(ev);
    this->setStyleSheet(oldStyle);
}

void PasswdLevelLabel::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        PasswdLevelChenged(currentLevel_);
        break;
    default:
        break;
    }
}

void PasswdLevelLabel::PasswdLevelChenged(int level)
{
    currentLevel_ = level;
    if(level == ICParametersSave::MachineOperator)
    {
        setText(tr("Machine Operator"));
    }
    else if(level == ICParametersSave::MachineAdmin)
    {
        setText(tr("Machine Admin"));
    }
    else if(level == ICParametersSave::AdvanceAdmin)
    {
        setText(tr("Advance Admin"));
    }
    emit Levelchenged(currentLevel_);
}
