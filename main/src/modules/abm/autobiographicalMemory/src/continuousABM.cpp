#include "autobiographicalMemory.h"

using namespace std;
using namespace yarp::os;

Bottle autobiographicalMemory::addContDataProvider(const string &type, const string &portDataContProvider)
{
    Bottle bReply;

    if (mapContDataProvider.find(type) == mapContDataProvider.end()) //key not found
    {
        mapContDataProvider[type] = portDataContProvider;
        mapContDataReceiver[type] = new yarp::os::BufferedPort < yarp::os::Bottle > ;

        bReply.addString("[ack]");
    }
    else { //key found
        string error = "ERROR: addDataContProvider: " + type + " is already present!";
        cout << error << endl;
        bReply.addString(error);
    }

    return bReply;
}

Bottle autobiographicalMemory::removeContDataProvider(const string &type)
{
    Bottle bReply;

    if (mapContDataProvider.find(type) == mapContDataProvider.end()) { //key not found
        string error = "ERROR: removeContDataProvider: " + type + ") is NOT present!";
        cout << error << endl;
        bReply.addString(error);
    }
    else { //key found
        mapContDataProvider.erase(type);
        mapContDataReceiver[type]->interrupt();
        mapContDataReceiver[type]->close();
        mapContDataReceiver.erase(type);
        bReply.addString("[ack]");
    }

    return bReply;
}

Bottle autobiographicalMemory::connectContDataProviders()
{
    Bottle bOutput;

    if (mapContDataProvider.size() == 0){
        bOutput.addString("ERROR [connectContDataProvider] the map is NULL");
        return bOutput;
    }

    for (std::map<string, string>::const_iterator it = mapContDataProvider.begin(); it != mapContDataProvider.end(); ++it)
    {
        string portContDataReceiver = "/" + getName() + "/contdata/" + it->first + "/in";
        mapContDataReceiver.find(it->first)->second->open(portContDataReceiver);
        //it->second: port name of continuous data Provider
        //mapContDataReceiver.find(it->first)->second: portname of contDataReceiver which correspond to the label of contDataProvider
        //cout << "  [connectContDataProvider] : trying to connect " << it->second << " with " <<  mapContDataReceiver.find(it->first)->second->getName().c_str() << endl ;
        if (!Network::isConnected(it->second, mapContDataReceiver.find(it->first)->second->getName().c_str())) {
            //cout << "Port is NOT connected : we will connect" << endl ;
            if (!Network::connect(it->second, mapContDataReceiver.find(it->first)->second->getName().c_str())) {
                cout << "Error: Connection could not be setup" << endl;
                bOutput.addString(it->second);
            }
            //cout << "Connection from : " << it->second << endl ;
            //cout << "Connection to   : " << mapContDataReceiver.find(it->first)->second->getName().c_str() << endl;
        }
        else {
            //cout << "Error: Connection already present!" << endl ;
        }
    }

    if (bOutput.size() == 0){
        bOutput.addString("ack");
    }

    return bOutput;
}

Bottle autobiographicalMemory::disconnectContDataProviders()
{
    Bottle bOutput;
    bool isAllDisconnected = true;

    if (mapContDataProvider.size() == 0){
        bOutput.addString("ERROR [disconnectContDataProviders] the map is NULL");
        return bOutput;
    }

    for (std::map<string, string>::const_iterator it = mapContDataProvider.begin(); it != mapContDataProvider.end(); ++it)
    {
        //it->second: port name of cont data Provider
        //mapContDataReceiver.find(it->first)->second: port name of contDataReceiver which correspond to the label of contDataProvider
        Network::disconnect(it->second, mapContDataReceiver.find(it->first)->second->getName().c_str());
        if (Network::isConnected(it->second, mapContDataReceiver.find(it->first)->second->getName().c_str())) {
            cout << "ERROR [disconnectContDataProviders] " << it->second << " is NOT disconnected!";
            bOutput.addString(it->second);
            isAllDisconnected = false;
        }
        else {
            //cout << "[disconnectContDataProviders] " << it->second << " successfully disconnected!"  << endl ;

            //Have to close/interrupt each time otherwise the port is not responsive anymore
            mapContDataReceiver.find(it->first)->second->interrupt();
            mapContDataReceiver.find(it->first)->second->close();
        }
    }

    if (isAllDisconnected == true){
        bOutput.addString("ack");
    }

    //cout << "[disconnectContDataProviders] bOutput = {" << bOutput.toString().c_str() << "}" << endl ;
    return bOutput;
}

bool autobiographicalMemory::storeContData(int instance, const string &type, int subtype, const string &contDataTime, const string &contDataPort, double value)
{
    Bottle bRequest;
    ostringstream osArg;

    bRequest.addString("request");
    osArg << "INSERT INTO continuousdata(instance, type, subtype, time, label_port, value) VALUES (" << instance << ", '" << type << "', '" << subtype << "', '" << contDataTime << "', '" << contDataPort << "', '" << value << "' );";
    bRequest.addString(osArg.str());
    bRequest = request(bRequest);

    return true;
}

bool autobiographicalMemory::storeContDataAllProviders(const string &synchroTime) {
    for (std::map<string, BufferedPort<Bottle>*>::const_iterator it = mapContDataReceiver.begin(); it != mapContDataReceiver.end(); ++it)
    {
        string type = it->first;
        Bottle* lastReading = it->second->read();

        if(lastReading != NULL) {
            for(int subtype = 0; subtype < lastReading->size(); subtype++) {
                storeContData(imgInstance, type, subtype, synchroTime, mapContDataProvider[it->first], lastReading->get(subtype).asDouble());
            }
        }
    }

    return true;
}

// From here on all send stream related
int autobiographicalMemory::openSendContDataPorts(int instance) {
    Bottle bDistLabelPort;
    ostringstream osArg;

    bDistLabelPort.addString("request");
    osArg << "SELECT DISTINCT label_port FROM continuousdata WHERE instance = " << instance << endl;
    bDistLabelPort.addString(osArg.str());
    bDistLabelPort = request(bDistLabelPort);

    for (int i = 0; i < bDistLabelPort.size() && bDistLabelPort.toString()!="NULL"; i++) {
        string contDataPort = bDistLabelPort.get(i).asList()->get(0).asString();
        mapContDataPortOut[contDataPort] = new yarp::os::BufferedPort < Bottle >;
        mapContDataPortOut[contDataPort]->open((portPrefix+contDataPort).c_str());
    }

    cout << "openSendContDataPorts just created " << mapContDataPortOut.size() << " ports." << endl;

    return mapContDataPortOut.size();
}

unsigned int autobiographicalMemory::getContDataProviderCount(int instance) {
    Bottle bRequest;
    ostringstream osArg;

    bRequest.addString("request");
    osArg << "SELECT DISTINCT label_port, type, subtype FROM continuousdata WHERE instance = " << instance;
    bRequest.addString(osArg.str());
    bRequest = request(bRequest);
    if(bRequest.size()>0 && bRequest.toString()!="NULL") {
        return bRequest.size();
    } else {
        return 0;
    }
}

long autobiographicalMemory::getTimeLastContData(int instance) {
    Bottle bRequest;
    ostringstream osArg;

    osArg << "SELECT CAST(EXTRACT(EPOCH FROM time-(SELECT time FROM continuousdata WHERE instance = " << instance << " ORDER BY time LIMIT 1)) * 1000000 as INT) as time_difference FROM continuousdata WHERE instance = " << instance << " ORDER BY time DESC LIMIT 1;";
    bRequest.addString("request");
    bRequest.addString(osArg.str());
    bRequest = request(bRequest);
    if(bRequest.size()>0 && bRequest.toString()!="NULL") {
        return atol(bRequest.get(0).asList()->get(0).asString().c_str());
    } else {
        return 0;
    }
}

Bottle autobiographicalMemory::getListContData(long updateTimeDifference) {
    Bottle bListContData;
    bListContData.addString("request");
    ostringstream osArgContData;

    osArgContData << "SELECT * FROM (";
    osArgContData << "SELECT subtype, label_port, time, value, ";
    osArgContData << "CAST(EXTRACT(EPOCH FROM time-(SELECT time FROM continuousdata WHERE instance = '" << imgInstance << "' ORDER BY time LIMIT 1)) * 1000000 as INT) as time_difference ";
    osArgContData << "FROM continuousdata WHERE instance = '" << imgInstance << "' ORDER BY time) s ";
    if(timingEnabled) {
        osArgContData << "WHERE time_difference <= " << updateTimeDifference << " and time_difference > " << timeLastImageSent << " ORDER BY time, label_port, subtype DESC LIMIT " << contDataProviderCount << ";";
    } else {
        osArgContData << "WHERE time_difference > " << timeLastImageSent << " ORDER BY time, label_port, subtype ASC LIMIT " << contDataProviderCount << ";";
    }

    bListContData.addString(osArgContData.str());
    return request(bListContData);
}
