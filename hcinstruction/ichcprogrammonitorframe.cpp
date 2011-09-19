#include "ichcprogrammonitorframe.h"
#include "ui_ichcprogrammonitorframe.h"


#include "icinstructparam.h"
#include "icparameterconversion.h"
#include "icvirtualhost.h"
#include "icautorunrevise.h"
#include "icactioncommand.h"
#include "icmacrosubroutine.h"
#include "icvirtualkey.h"

ICHCProgramMonitorFrame::ICHCProgramMonitorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICHCProgramMonitorFrame),
    isModify_(false),
    oldTime_(-1),
    currentStepItem_(NULL),
    currentMoldNum_(8),
    isFollow_(true)
{
    ui->setupUi(this);

    autoRunRevise_ = new ICAutoRunRevise();
    InitSignal();
    UpdateHostParam();
    //    ICInstructParam::Instance()->UpdateHostParam();
    //    moldContent_ = ICMold::CurrentMold()->MoldContent();
}

ICHCProgramMonitorFrame::~ICHCProgramMonitorFrame()
{
    delete autoRunRevise_;
    delete ui;
}

void ICHCProgramMonitorFrame::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ICHCProgramMonitorFrame::showEvent(QShowEvent *e)
{
    SetProduct(ICMold::CurrentMold()->MoldParam(ICMold::Product));
    UpdateHostParam();
    QFrame::showEvent(e);
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StepChanged(int)),
            this,
            SLOT(SelectCurrentStep(int)));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(StatusRefreshed()),
            this,
            SLOT(StatusRefreshed()));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(SubStepChanged(uint8_t*)),
            this,
            SLOT(SubStepChanged(uint8_t*)));
    connect(ICVirtualHost::GlobalVirtualHost(),
            SIGNAL(MoldNumChanged(int)),
            this,
            SLOT(MoldNumChanged(int)));

    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnAuto,
                                                     0,
                                                     ICMold::CurrentMold()->SyncAct() + ICMacroSubroutine::Instance()->SyncAct(),
                                                     ICMold::CurrentMold()->SyncSum() + ICMacroSubroutine::Instance()->SyncSum());
}

void ICHCProgramMonitorFrame::hideEvent(QHideEvent *e)
{
    qDebug("isModify change to false in hide");
    isModify_ = false;
    QFrame::hideEvent(e);
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StepChanged(int)),
               this,
               SLOT(SelectCurrentStep(int)));
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(StatusRefreshed()),
               this,
               SLOT(StatusRefreshed()));
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(SubStepChanged(uint8_t*)),
               this,
               SLOT(SubStepChanged(uint8_t*)));
    disconnect(ICVirtualHost::GlobalVirtualHost(),
               SIGNAL(MoldNumChanged(int)),
               this,
               SLOT(MoldNumChanged(int)));
    //    ICCommandProcessor::Instance()->ExecuteHCCommand(IC::CMD_TurnStop,0);
}

void ICHCProgramMonitorFrame::SetTime(int time)
{
    ui->timeLabel->setText(ICParameterConversion::TransThisIntToThisText(time, 1));
}

//void ICHCProgramMonitorFrame::SetFullTime(int fullTime)
//{
//    ui->fullTimeLabel->setText(ICParameterConversion::TransThisIntToThisText(fullTime, 1));
//}

void ICHCProgramMonitorFrame::SetProduct(int product)
{
    ui->settedProductsLabel->setText(QString::number(product));
}

//void ICHCProgramMonitorFrame::SetCurrentFinished(int currentFinished)
//{
//    ui->currentProductsLabel->setText(QString::number(currentFinished));
//}

void ICHCProgramMonitorFrame::StatusRefreshed()
{
    ICVirtualHost* host = ICVirtualHost::GlobalVirtualHost();
    newTime_ = host->HostStatus(ICVirtualHost::Time).toInt();
    if(newTime_ != oldTime_)
    {
        oldTime_ = newTime_;
        SetTime(newTime_);
    }
    if(host->CurrentStatus() != ICVirtualHost::Auto)
    {
        qDebug("isModify change to false in auto");
        isModify_ = false;
    }
    if(host->HostStatus(ICVirtualHost::DbgX0) != ICVirtualHost::AutoRunning)
    {
        qDebug("isModify change to false in autoRunning");
        isModify_ = false;
    }
}

void ICHCProgramMonitorFrame::SelectCurrentStep(int currentStep)
{
    if(currentStep == 0 && isModify_)
    {
        ICMold::CurrentMold()->SetMoldContent(ICMold::UIItemToMoldItem(programList_));
        ICMold::CurrentMold()->SaveMoldFile();
        qDebug("run modify");
        UpdateHostParam();
        isModify_ = false;
    }
    if(currentStep < 0 || currentStep >= programList_.size())
    {
        qDebug()<<"current step wrong"<<currentStep;
        return;
    }
    if(!isFollow_)
    {
        return;
    }
    ui->moldContentListWidget->clearSelection();
    ICGroupMoldUIItem* gItem = &programList_[currentStep];
    currentStepItem_ = gItem;
    int selectedCount = gItem->ItemCount();
    startIndex_ = 0;
    for(int i = 0; i != currentStep; ++i)
    {
        startIndex_ += programList_.at(i).ItemCount();
    }
    if(startIndex_ < ui->moldContentListWidget->count())
    {
        ui->moldContentListWidget->scrollToItem(ui->moldContentListWidget->item(startIndex_));
    }
    const int topItemCount = gItem->TopItemCount();
    int nextTopItemIndex = startIndex_;
    for(int i = 0; i != topItemCount; ++i)
    {
        if(nextTopItemIndex < ui->moldContentListWidget->count())
        {
            ui->moldContentListWidget->item(nextTopItemIndex)->setSelected(true);
        }
        if(gItem->at(i).SubItemCount() != 0)
        {
            nextTopItemIndex += gItem->at(i).ItemCount();
        }
        else
        {
            ++nextTopItemIndex;
        }
    }
}

void ICHCProgramMonitorFrame::SubStepChanged(uint8_t* subStep)
{
    if(!isFollow_)
    {
        return;
    }
    if(currentStepItem_ == NULL)
    {
        return;
    }
    const int topItemCount = currentStepItem_->TopItemCount();
    int currentSub = 0;
    int nextTopItemIndex = startIndex_ + 1;
    int size;
    for(int i = 0; i != topItemCount; ++i)
    {
        if(currentStepItem_->at(i).SubItemCount() != 0)
        {
            if(subStep[currentSub] != 0)
            {
                size = nextTopItemIndex + subStep[currentSub] - 1;
                if(size < ui->moldContentListWidget->count())
                {
                    ui->moldContentListWidget->item(nextTopItemIndex + subStep[currentSub] - 1)->setSelected(false);
                }
            }
            size = nextTopItemIndex + (subStep[currentSub]);
            if(size < ui->moldContentListWidget->count())
            {
                ui->moldContentListWidget->item(nextTopItemIndex + (subStep[currentSub++]))->setSelected(true);
            }
            nextTopItemIndex += currentStepItem_->at(i).ItemCount();
        }
        else
        {
            ++nextTopItemIndex;
        }
    }

}

void ICHCProgramMonitorFrame::UpdateHostParam()
{
    qDebug("m update");
    startIndex_ = 0;
    currentStepItem_ = NULL;
    if(currentMoldNum_ == 8)
    {
        programList_ = ICMold::CurrentMold()->ToUIItems();
    }
    else if(currentMoldNum_ >= 0 && currentMoldNum_ < 8)
    {
        programList_ = ICMold::MoldItemToUIItem(ICMacroSubroutine::Instance()->SubRoutine(currentMoldNum_));
    }
    else
    {
        return;
    }
    qDebug("after m update");

    UpdateUIProgramList_();
}

void ICHCProgramMonitorFrame::InitSignal()
{
    //    connect(ICInstructParam::Instance(),
    //            SIGNAL(HostParamChanged()),
    //            this,
    //            SLOT(UpdateHostParam()));
    //    connect(autoRunRevise_,
    //            SIGNAL(MoldItemChanged()),
    //            this,
    //            SLOT(MoldItemChanged()));
}


void ICHCProgramMonitorFrame::on_editToolButton_clicked()
{
    const int selectedRow = ui->moldContentListWidget->currentRow();
    if(selectedRow < 0)
    {
        return;
    }
    int gIndex;
    int tIndex;
    int sIndex;
    FindIndex_(selectedRow, gIndex, tIndex, sIndex);
    if(sIndex < 0)
    {
        ICTopMoldUIItem * topItem = &programList_[gIndex].at(tIndex);
        ICMoldItem* item = topItem->BaseItem();
        bool isM = autoRunRevise_->ShowModifyItem(item,topItem->ToStringList().join("\n"));
        qDebug()<<"after show"<<isM;
        isModify_ = isModify_ || isM;
        //        {
        //            ui->moldContentListWidget->item(selectedRow)->setText(topItem->ToStringList().at(0));
        //        }
    }
    else
    {
        ICSubMoldUIItem *subItem = &programList_[gIndex].at(tIndex).at(sIndex);
        bool isM = autoRunRevise_->ShowModifyItem(subItem->BaseItem(), subItem->ToString());
        qDebug()<<"after show"<<isM;
        isModify_ = isModify_ || isM;//        if(isModify)
        //        {
        //            ui->moldContentListWidget->item(selectedRow)->setText(subItem->ToString());
        //        }

    }
    qDebug()<<"Modify"<<isModify_;
    //    int currentRow = ui->moldContentListWidget->currentRow();
    //    if(currentRow < 0)
    //    {
    //        return;
    //    }
    //    autoRunRevise_->SetMoldItem(&(moldContent_[currentRow]));
    //    autoRunRevise_->open();
}

void ICHCProgramMonitorFrame::MoldItemChanged()
{
    //    ICMold::Modify(QList<ICMoldItem>()<<item, moldContent_);

    isModify_ = true;
}

void ICHCProgramMonitorFrame::UpdateUIProgramList_()
{
    ui->moldContentListWidget->clear();
    qDebug("after clear");

    ui->moldContentListWidget->addItems(ICMold::UIItemsToStringList(programList_));
    ICGroupMoldUIItem groupItem;
    int topItemRowCount;
    int index = 0;
    QColor color;
    for(int i = 0; i != programList_.size(); ++i)
    {
        (i % 2 == 0 ? color.setRgb(255, 255, 154):color.setRgb(154, 255, 255));
        groupItem = programList_.at(i);
        topItemRowCount = groupItem.ItemCount();
        for(int j = 0; j != topItemRowCount; ++j)
        {
            ui->moldContentListWidget->item(j + index)->setBackgroundColor(color);
        }
        index += topItemRowCount;
    }
    qDebug("updatge end");
}

void ICHCProgramMonitorFrame::on_singleStepButton_clicked()
{
    ICCommandProcessor::Instance()->ExecuteVirtualKeyCommand(IC::VKEY_F6);
}

void ICHCProgramMonitorFrame::FindIndex_(int currentIndex, int& groupItemIndex, int &topItemIndex, int &subItemIndex)
{
    for(int i = 0; i != programList_.size(); ++i)
    {
        if(currentIndex < programList_.at(i).ItemCount())
        {
            groupItemIndex = i;
            for(int j = 0; j != programList_.at(i).TopItemCount(); ++j)
            {
                if(currentIndex < programList_.at(i).at(j).ItemCount())
                {
                    topItemIndex = j;
                    if(currentIndex == 0)
                    {
                        subItemIndex = -1;
                    }
                    else
                    {
                        subItemIndex = currentIndex - 1;
                    }
                    break;
                }
                else
                {
                    currentIndex -= programList_.at(i).at(j).ItemCount();
                }
            }
            break;
        }
        else
        {
            currentIndex -= programList_.at(i).ItemCount();
        }
    }
}

void ICHCProgramMonitorFrame::MoldNumChanged(int mold)
{
    this->blockSignals(true);
    currentMoldNum_ = mold;
    UpdateHostParam();
    this->blockSignals(false);
}

void ICHCProgramMonitorFrame::on_followToolButton_clicked()
{
    if(isFollow_)
    {
        isFollow_ = false;
        ui->followToolButton->setIcon(QPixmap(":/resource/stop.png"));
        ui->followToolButton->setText(tr("No Follow"));
    }
    else
    {
        isFollow_ = true;
        ui->followToolButton->setIcon(QPixmap(":/resource/play.png"));
        ui->followToolButton->setText(tr("Follow"));
    }
}
