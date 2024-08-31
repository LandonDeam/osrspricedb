// Copyright Landon Deam 2024

#pragma once
#ifndef GET_PRICES_H_
#define GET_PRICES_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/Context.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"

const Poco::URI uri("https://prices.runescape.wiki");

#endif  // GET_PRICES_H_
