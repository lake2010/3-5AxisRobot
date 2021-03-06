#include <QApplication>
#include "icmonitorpageframe.h"
#include "ui_icmonitorpageframe.h"
#include "iciopoint.h"
#include "iciomonitorpagebase.h"
#include "config.h"

ICMonitorPageFrame::ICMonitorPageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ICMonitorPageFrame)
{
    ui->setupUi(this);
    otherPageLeft_ = new ICIOMonitorPageBase();
    otherPageRight_ = new ICIOMonitorPageBase();
    euPageLeft_ = new ICIOMonitorPageBase();
    euPageRight_ = new ICIOMonitorPageBase();
    Init_();
}

ICMonitorPageFrame::~ICMonitorPageFrame()
{
    delete ui;
}

void ICMonitorPageFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        Init_();
        break;
    default:
        break;
    }
}

void ICMonitorPageFrame::Init_()
{
    ui->icMonitorFrameleft->Clear();
    ui->icMonitorFrameRight->Clear();
    QList<ICIOPoint> points;
    points.append(ICIOPoint("X010", tr("Horizontal-1"), 0));
    points.append(ICIOPoint("X011", tr("Vertical-1"), 1));
    points.append(ICIOPoint("X012", tr("Fixtur1"), 2));//夹1确认限
    points.append(ICIOPoint("X013", tr("Fixtur2"), 3));
    points.append(ICIOPoint("X014", tr("Sucker2"), 4));
    points.append(ICIOPoint("X015", tr("Sucker1"), 5));//吸1确认限
    points.append(ICIOPoint("X016", tr("X1 Out Limit"), 6));
    points.append(ICIOPoint("X017", tr("X1 In Limit"), 7));

    points.append(ICIOPoint("X020", tr("X1 Origin"), 8));
    points.append(ICIOPoint("X021", tr("Fixtur4"), 9));
    points.append(ICIOPoint("X022", tr("Y1 In Limit"), 10));
    points.append(ICIOPoint("X023", tr("A Origin/Horizontal-2"), 11));
    points.append(ICIOPoint("X024", tr("Out Safe Area"), 12));
    points.append(ICIOPoint("X025", tr("Come In Origin"), 13));
//    points.append(ICIOPoint("X026", tr("Reserve"), 14));
    points.append(ICIOPoint("X026", tr("B Origin Limit"), 14));
    points.append(ICIOPoint("X027", tr("Pressure"), 15));

    points.append(ICIOPoint("X030", tr("X2 In Limit"), 16));
    points.append(ICIOPoint("X031", tr("X2 Origin"), 17));
    points.append(ICIOPoint("X032", tr("Y2 Origin"), 18));
    points.append(ICIOPoint("X033", tr("Y2 In Limit"), 19));
    points.append(ICIOPoint("X034", tr("Fixture3"), 20));
    points.append(ICIOPoint("X035", tr("Y1 Origin"), 21));
    points.append(ICIOPoint("X036", tr("Y1 Out Limit"), 22));
    points.append(ICIOPoint("X037", tr("Vertical-2"), 23));

    points.append(ICIOPoint("X040", tr("B Origin"), 24));
    points.append(ICIOPoint("X041", tr("Z In Limit"), 25));
    points.append(ICIOPoint("X042", tr("Z Out Limit"), 26));
    points.append(ICIOPoint("X043", tr("Check In0"), 27));
    points.append(ICIOPoint("X044", tr("Check In1"), 28));
    points.append(ICIOPoint("X045", tr("Watch Dog"), 29));
    points.append(ICIOPoint("X046", tr("Next Cycle App"), 30));
    points.append(ICIOPoint("X047", tr("Mold Close App"), 31));

//    points.append(ICIOPoint("X050", tr("X1 Servo Alarm"), 48));
//    points.append(ICIOPoint("X051", tr("Y1 Servo Alarm"), 49));
//    points.append(ICIOPoint("X052", tr("Z Servo Alarm"), 50));
//    points.append(ICIOPoint("X053", tr("X2 Servo Alarm"), 51));
//    points.append(ICIOPoint("X054", tr("Y2 Servo Alarm"), 52));
//    points.append(ICIOPoint("X055", tr("A Servo Alarm"), 53));
//    points.append(ICIOPoint("X056", tr("B Servo Alarm"), 54));
//    points.append(ICIOPoint("X057", tr("C Servo Alarm"), 55));
    ui->icMonitorFrameleft->AddPage(otherPageLeft_, tr("Other"));
    ui->icMonitorFrameRight->AddPage(otherPageRight_, tr("Other"));
    otherPageLeft_->BindingPointsToInputPage(points);
    otherPageRight_->BindingPointsToInputPage(points);
    points.clear();

    points.append(ICIOPoint("Y010", tr("Horizontal-1 Valve"), 0));
    points.append(ICIOPoint("Y011", tr("Vertical-1 Valve"), 1));
    points.append(ICIOPoint("Y012", tr("Fixtur1 Valve"), 2));  //夹1阀
    points.append(ICIOPoint("Y013", tr("Fixtur2 Valve"), 3));
    points.append(ICIOPoint("Y014", tr("Sucker2 Valve"), 4));
    points.append(ICIOPoint("Y015", tr("Sucke1 Valve"), 5));
    points.append(ICIOPoint("Y016", tr("Main Forward Valve"), 6));
    points.append(ICIOPoint("Y017", tr("Adjust Sub Back"), 7));

    points.append(ICIOPoint("Y020", tr("Fixtur4 Valve"), 8));
    points.append(ICIOPoint("Y021", tr("Alarm"), 9));
    points.append(ICIOPoint("Y022", tr("Adjust Sub Forward"), 10));//1
    points.append(ICIOPoint("Y023", tr("Next Cycle Sp2"), 11));//备用输出2
    points.append(ICIOPoint("Y024", tr("Horizontal-2 Valve"), 12));
    points.append(ICIOPoint("Y025", tr("Paint Oil"), 13));
    points.append(ICIOPoint("Y026", tr("Next Cycle Sp"), 14));
    points.append(ICIOPoint("Y027", tr("Mold Close Sp"), 15));

    points.append(ICIOPoint("Y030", tr("Vice Forward Valve"), 16));
    points.append(ICIOPoint("Y031", tr("Adjust Sub Up"), 17));
    points.append(ICIOPoint("Y032", tr("Adjust Sub Down"), 18));
    points.append(ICIOPoint("Y033", tr("Vice Sub Down"), 19));
    points.append(ICIOPoint("Y034", tr("Fixture3 Valve"), 20));
    points.append(ICIOPoint("Y035", tr("Machine Reversal"), 21));//4
    points.append(ICIOPoint("Y036", tr("Run Led"), 22));//3
    points.append(ICIOPoint("Y037", tr(" Transport"), 23));

    points.append(ICIOPoint("Y040", tr("Y040"), 24));
    points.append(ICIOPoint("Y041", tr("Y041"), 25));
    points.append(ICIOPoint("Y042", tr("Y042"), 26));
    points.append(ICIOPoint("Y043", tr("Y043"), 27));
    points.append(ICIOPoint("Y044", tr("Y044"), 28));
    points.append(ICIOPoint("Y045", tr("Y045"), 29));//4
    points.append(ICIOPoint("Y046", tr("Y046"), 30));//3
    points.append(ICIOPoint("Y047", tr("Y047"), 31));
//#ifdef HC_8AXIS
//    points.append(ICIOPoint("Y022", tr("Reserve 1"), 10));//1
//    points.append(ICIOPoint("Y023", tr("Reserve 2"), 11));//2
//    points.append(ICIOPoint("Y032", tr("Reserve 3"), 18));
//    points.append(ICIOPoint("Y033", tr("Reserve 4"), 19));
//    points.append(ICIOPoint("Y035", tr("Reserve 5"), 21));//4
//    points.append(ICIOPoint("Y036", tr("Reserve 6"), 22));//3
//#endif
    otherPageLeft_->BindingPointsToOutpuPage(points);
    otherPageRight_->BindingPointsToOutpuPage(points);

    ui->icMonitorFrameleft->AddPage(euPageLeft_, tr("EU"));
    ui->icMonitorFrameRight->AddPage(euPageRight_, tr("EU"));

    points.clear();
    points.append(ICIOPoint("", tr("Mold Opened"), 35));
    points.append(ICIOPoint("", tr("Mold Closed"), 34));
    points.append(ICIOPoint("", tr("Eject Forward"), 39));
    points.append(ICIOPoint("", tr("Eject Backward"), 38));
//    points.append(ICIOPoint("", tr("Mode open In 2"), 46)); //开模2完
    points.append(ICIOPoint("", tr("Core 1 In"), 46)); //入芯1到位
    points.append(ICIOPoint("", tr("Core 1 Out"), 45));
    points.append(ICIOPoint("", tr("Core 2 In"), 44));
    points.append(ICIOPoint("", tr("Core 2 Out"), 43));

//    points.append(ICIOPoint("", tr("Safe Door 2"), 33));//安全门2
    points.append(ICIOPoint("", tr("IMM Reject"), 33));//不良品
    points.append(ICIOPoint("", tr("ZC5"), 42));
    points.append(ICIOPoint("", tr("ZC6"), 41));
    points.append(ICIOPoint("", tr("ZC7"), 40));
    points.append(ICIOPoint("", tr("Safe Door"), 32));
    points.append(ICIOPoint("", tr("ES"), 47));
    points.append(ICIOPoint("", tr("Mold Inter"), 36));
    points.append(ICIOPoint("", tr("IMM Auto"), 37));

    euPageLeft_->BindingPointsToInputPage(points);
    euPageRight_->BindingPointsToInputPage(points);

    points.clear();
    points.append(ICIOPoint("", tr("Mold Open Permit"), 38)); //允许开模
    points.append(ICIOPoint("", tr("Mold Close Permit"), 32));
    points.append(ICIOPoint("", tr("Eject Forward Permit"), 33));
//    points.append(ICIOPoint("", tr("Thimble Permit 1"), 33));
    points.append(ICIOPoint("", tr("Eject Backward Permit"), 37));
//    points.append(ICIOPoint("", tr("Thimble Permit 2"), 37));
//    points.append(ICIOPoint("", tr("Mode Close Permit 2"), 40));
    points.append(ICIOPoint("", tr("Core 1 In Permit"), 40));
    points.append(ICIOPoint("", tr("Core 1 Out Permit"), 46));
    points.append(ICIOPoint("", tr("Core 2 In Permit"), 41));
    points.append(ICIOPoint("", tr("Core 2 Out Permit"), 45));

    points.append(ICIOPoint("", tr("Robot"), 35));
    points.append(ICIOPoint("", tr("En C6"), 44));
    points.append(ICIOPoint("", tr("En C7"), 43));
    points.append(ICIOPoint("", tr("En A8"), 42));
//    points.append(ICIOPoint("", tr("Blank 0"), 34));
//    points.append(ICIOPoint("", tr("Blank 1"), 36));
//    points.append(ICIOPoint("", tr("Blank2"), 39));
//    points.append(ICIOPoint("", tr("Blank3"), 47));

    euPageLeft_->BindingPointsToOutpuPage(points);
    euPageRight_->BindingPointsToOutpuPage(points);
}
