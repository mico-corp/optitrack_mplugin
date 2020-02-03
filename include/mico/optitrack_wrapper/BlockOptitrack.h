//---------------------------------------------------------------------------------------------------------------------
//  mico
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2019 Ricardo Lopez Lopez (a.k.a Ric92) ricloplop@gmail.com & Emanuela Savastano (a.k.a emasava)
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

#ifndef MICO_OPTITRACK_WRAPPER_BLOCK_OPTITRACK_H_
#define MICO_OPTITRACK_WRAPPER_BLOCK_OPTITRACK_H_

#include <flow/flow.h>
#include <flow/DataFlow.h>

namespace optitrack_wrapper{
    class BlockOptitrack : public flow::Block{
    public:
		BlockOptitrack();

        std::string name() const override{return "optitrack"; }
        std::string description() const override {return    "Communication block using Optitrack.\n";};
        
        virtual QWidget * customWidget() override;

        virtual bool configure(std::unordered_map<std::string, std::string> _params) override{
            serverIp_ = _params["ServerId"];
            clientIp_ = _params["ClientId"];
            return true;
        };

        std::vector<std::string> parameters() override{
            return {
			    "ServerId", "ClientId" 
            };
        };

    protected:
        virtual void loopCallback() override;

    private:
        std::string serverIp_;
        std::string clientIp_;
        int targetRate_ = 120;
    };
}
#endif