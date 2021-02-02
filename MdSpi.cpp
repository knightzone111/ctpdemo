#include "MdSpi.h"
#include <iostream>
#include <cstring>

char* BROKER_ID="9999"; // xzqh // SimNow: 9999
char* USER_ID="";
char* PASSWORD="";
char* INSTRUMENTS[] = {"ag2105", "au2105", "ru2105"};
int INSTRUMENT_COUNT = 3;

void MdSpi::OnFrontConnected()
{
    std::cout << "OnFrontConnected" << std::endl;
    ReqUserLogin();
}

void MdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
                       int nRequestID, bool bIsLast)
{
    std::cerr << "OnRspError" << std::endl;
    IsErrorRspInfo(pRspInfo);
}

void MdSpi::OnHeartBeatWarning(int nTimeLapse)
{
    std::cerr << "OnHeartBeatWarning: nTimerLapse = " << nTimeLapse << std::endl;
}

void MdSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    std::memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, USER_ID);
    strcpy(req.Password, PASSWORD);
    int ret = api->ReqUserLogin(&req, ++requestId);
    std::cout << "ReqUserLogin: " << ret << std::endl;
}

void MdSpi::OnRspUserLogin(
    CThostFtdcRspUserLoginField *pRspUserLogin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cout << "OnRspUserLogin" << std::endl;
    std::cout << "ErrorID: " << pRspInfo->ErrorID << std::endl;
    std::cout << "TradingDay: " << pRspUserLogin->TradingDay << std::endl;
    std::cout << "SessionID: " << pRspUserLogin->SessionID << std::endl;
    std::cout << "FrontID: " << pRspUserLogin->FrontID << std::endl;
    std::cout << "MaxOrderRef: " << pRspUserLogin->MaxOrderRef << std::endl;

    if (pRspInfo->ErrorID == 0 && bIsLast)
    {
        SubscribeMarketData();
    }
}

void MdSpi::SubscribeMarketData()
{
    int ret = api->SubscribeMarketData(INSTRUMENTS, INSTRUMENT_COUNT);
    std::cout << "SubscribeMarketData: " << ret << std::endl;
}

void MdSpi::OnRspSubscribeMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cout << "OnRspSubscribeMarketData" << std::endl;
}

void MdSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cout << "OnRspQryDepthMarketData" << std::endl;
}

void MdSpi::OnRtnDepthMarketData(
    CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    std::cout << "OnRtnDepthMarketData: " << pDepthMarketData->InstrumentID << ", " << pDepthMarketData->UpdateTime << ", " << pDepthMarketData->UpdateMillisec << ", " <<
    pDepthMarketData->LastPrice <<  std::endl;
}

void MdSpi::OnFrontDisconnected(int nReason)
{
    std::cout << "OnFrontDisconnected: " << nReason << std::endl;
}

bool MdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
        std::cerr << "ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
    return bResult;
}
