//---------------------------------------------------------------------------------------------------------------------
//  mico
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2020 Ricardo Lopez Lopez (a.k.a ric92) & Emanuela Savastano (a.k.a emasava)
//---------------------------------------------------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
//  and associated documentation files (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify, merge, publish, distribute,
//  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all copies or substantial
//  portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
//  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------


#include <mico/optitrack_wrapper/BlockOptitrack.h>
#include <flow/Outpipe.h>
#include <eigen3/Eigen/Core>
#include <QSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>



namespace optitrack_wrapper{
        BlockOptitrack::BlockOptitrack(){
            createPipe("Pose","mat44");
        }

        // BlockOptitrack::~BlockOptitrack(){
        //     //Close Sockets
        //     close(optitrack->sdData);
        //     close(optitrack->sdCommand);
        // }

        QWidget * BlockOptitrack::customWidget(){
            QGroupBox *box = new QGroupBox;
            QHBoxLayout *layout = new QHBoxLayout;
            box->setLayout(layout);
            QLabel *label = new QLabel("Target Hz");
            layout->addWidget(label);

            QSpinBox *rateController = new QSpinBox;
            rateController->setMinimum(1);
            rateController->setValue(int(targetRate_));
            layout->addWidget(rateController);

            QWidget::connect(rateController, QOverload<int>::of(&QSpinBox::valueChanged), [this](int _val){
                targetRate_ = _val;
            });

            return box;
        }

        void BlockOptitrack::loopCallback() {
            auto t0  = std::chrono::steady_clock::now();
            optitrack_ = new Opti_wrap();

            optitrack_->start_communication();
            optitrack_->start_time_counter();
            while(isRunningLoop()){
                optitrack_->run(true);
                auto t1  = std::chrono::steady_clock::now();
                float incT = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
                if(incT / 1000 > 1/targetRate_){
                    t0 = t1;
                    Eigen::Matrix4f pose = Eigen::Matrix4f::Identity();
                    if(auto pipe = getPipe("Pose"); pipe->registrations() !=0 ){
                        optitrack_->print_frame_info("Maduro",(int)5,true);
                        //pipe->flush(pose); 
                    }
                }
            }         
        }
}