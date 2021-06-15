#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<cmath>
#include<string>
#include"Image.h"
#include"CheckerBoard.h"
#include"Constants.h"

#include"httplib.h"

#include<memory>
using namespace cv;
using namespace std;

#undef streamCam

std::array<std::array<bool, ConstantsValues::boardSize>, ConstantsValues::boardSize> tiles = {};
std::unique_ptr<CheckerBoard> cmpBoard;
#ifdef streamCam
cv::VideoCapture cap = cv::VideoCapture("http://localhost:12345");
#endif

CheckerBoard getCheckerboard(Image image, bool& success)
{
    CheckerBoard board = CheckerBoard(image);
    if (!board.CalculatePoints())
    {
        std::cout << "calculating failed" << std::endl;
        success = false;
        return board;
    }
    std::cout << "new image" << std::endl;
    success = true;
    return board;
}

std::string getBoard()
{
    std::string outpt = "";
    for (int y = 0; y < tiles[0].size(); y++)
    {
        for (size_t x = 0; x < tiles.size(); x++)
        {
            if (tiles[x][(tiles[x].size() - 1) - y])
            {
                outpt += "1 ";
            }
            else
            {
                outpt += "0 ";
            }
        }
        outpt += '\n';
    }
    return outpt;
}

cv::Mat getCamera()
{
    #ifdef streamCam
    cv::Mat picture;
    if (!cap.read(picture))
    {
        std::cout << "failed to read camera" << std::endl;
        return cv::Mat();
    }
    #else
    std::system("raspistill -t 1 -o capture.jpg");
    cv::Mat picture = imread("capture.jpg");
    #endif
    resize(picture, picture, Size(ConstantsValues::ImageSize.getValue(), ConstantsValues::ImageSize.getValue()));
    //cv::imshow("picture", picture); 
    //cv::waitKey(1);
    return picture;
}

bool VerifyChanged(CheckerBoard oldboard, CheckerBoard& newboard, std::array<std::array<bool, ConstantsValues::boardSize>, ConstantsValues::boardSize>& changedTiles)
{
    auto changed = std::vector<std::array<std::array<bool, ConstantsValues::boardSize>, ConstantsValues::boardSize>>();
    for (size_t i = 0; i < ConstantsValues::Verifies.getValue(); i++)
    {
        auto picture = getCamera();
        if (picture.empty())
        {
            return false;
        }

        auto img = Image(picture);
        //cv::imshow("img", picture);
        //cv::waitKey(100);
        auto sucess = false;
        auto checkBoard = getCheckerboard(img, sucess);
        if (!sucess)
        {
            return false;
        }

        auto fail = false;
        auto changes = checkBoard.getChanged(oldboard);
        newboard = checkBoard;
        changed.push_back(changes);
    }

    auto comp = changed[0];
    for (size_t i = 1; i < changed.size(); i++)
    {
        for (size_t x = 0; x < changed[i].size(); x++)
        {
            for (size_t y = 0; y < changed[i][x].size(); y++)
            {
                if (comp[x][y] != changed[i][x][y])
                {
                    return false;
                }
            }
        }
    }

    changedTiles = comp;
    return true;
}

void checker_recognition()
{
    while (true)
    {
	    if(cmpBoard == nullptr)
	    {
		    std::cout << "ptr null" << std::endl;
	    }
        CheckerBoard newboard = *cmpBoard;
        std::array<std::array<bool, ConstantsValues::boardSize>, ConstantsValues::boardSize> changedTiles;
        if (!VerifyChanged(*cmpBoard, newboard, changedTiles))
        {
            continue;
        }
        cmpBoard = std::make_unique<CheckerBoard>(newboard);
        
        for (size_t x = 0; x < changedTiles.size(); x++)
        {
            for (size_t y = 0; y < changedTiles[x].size(); y++)
            {
                tiles[x][y] ^= changedTiles[x][y];
            }
        }
        
        std::cout << getBoard() << std::endl;
        break;
    }
            
    return;
}

void setup_empty()
{
    while (cmpBoard == nullptr)
    {
        auto picture = getCamera();
        if (picture.empty())
        {
            continue;
        }

        auto img = Image(picture);

        auto sucess = false;
        auto newboard = getCheckerboard(img, sucess);
        if (!sucess)
        {
            continue;
        }

        CheckerBoard tmp = newboard;
        std::array<std::array<bool, ConstantsValues::boardSize>, ConstantsValues::boardSize> changedTiles;
        if (!VerifyChanged(newboard, tmp, changedTiles))
        {
            continue;
        }

        cmpBoard = std::make_unique<CheckerBoard>(newboard);
    }

    std::cout << "setup empty board complete" << std::endl;

}

int main(int argc, char *argv[])
{
    setup_empty();

    httplib::Server svr;

    svr.Get("/update", [](const httplib::Request&, httplib::Response& res) {
	    std::cout << "update" << std::endl;
        checker_recognition();
        res.set_content(getBoard(), "text/plain");
    });

    httplib::Client cli("http://192.168.1.253:12345");
    svr.Get("/set", [&cli](const httplib::Request& req, httplib::Response& res) {
        auto orgpos = req.get_param_value("org");
        auto newpos = req.get_param_value("new");
        std::string str = "/set?org=" + orgpos + "&new=" + newpos;
        auto resp = cli.Get((char *) str.c_str());//"/set?org=A3&new=B2"
        std::cout << resp->status << std::endl;
        std::cout << str << std::endl;
    });

    svr.Get("/hello", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    }); 
    svr.Get("/get", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(getBoard(), "text/plain");
    });

    svr.Get("/reset", [](const httplib::Request&, httplib::Response& res) {
        tiles = {};
        cmpBoard = nullptr;
        setup_empty();
        res.set_content(getBoard(), "text/plain");
    });

    svr.listen("0.0.0.0", ConstantsValues::port.getValue());
}
