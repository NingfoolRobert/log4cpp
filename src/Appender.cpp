/*
 * Appender.cpp
 *
 * Copyright 2000, LifeLine Networks BV (www.lifeline.nl). All rights reserved.
 * Copyright 2000, Bastiaan Bakker. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4cpp/Portability.hh>
#include <log4cpp/Appender.hh>

namespace log4cpp {
#ifdef LOG4CPP_USE_CLEANUP
    Log4cppCleanup& Appender::_fuckinDummy = Log4cppCleanup::_cleanup;
#endif

    Appender::AppenderMap* Appender::_allAppenders;
    threading::Mutex Appender::_appenderMapMutex;

    /* assume _appenderMapMutex locked */
    Appender::AppenderMap& Appender::_getAllAppenders() {
        if (!_allAppenders) 
            _allAppenders = new Appender::AppenderMap();

        return *_allAppenders;
    }

    Appender* Appender::getAppender(const std::string& name) {
        threading::ScopedLock lock(_appenderMapMutex);
        AppenderMap& allAppenders = Appender::_getAllAppenders();
        AppenderMap::iterator i = allAppenders.find(name);
        return (allAppenders.end() == i) ? NULL : ((*i).second);
    }
    
    void Appender::_addAppender(Appender* appender) {
        //REQUIRE(_allAppenders.find(appender->getName()) == _getAllAppenders().end())
        threading::ScopedLock lock(_appenderMapMutex);
        _getAllAppenders()[appender->getName()] = appender;
    }

    void Appender::_removeAppender(Appender* appender) {
        threading::ScopedLock lock(_appenderMapMutex);
        _getAllAppenders().erase(appender->getName());
    }
    
    bool Appender::reopenAll() {
        threading::ScopedLock lock(_appenderMapMutex);
        bool result = true;
        AppenderMap& allAppenders = _getAllAppenders();
        for(AppenderMap::iterator i = allAppenders.begin(); i != allAppenders.end(); i++) {
            result = result && ((*i).second)->reopen();
        }
        
        return result;
    }
    
    void Appender::closeAll() {
        threading::ScopedLock lock(_appenderMapMutex);
        AppenderMap& allAppenders = _getAllAppenders();
        for(AppenderMap::iterator i = allAppenders.begin(); i != allAppenders.end(); i++) {
            ((*i).second)->close();
        }
    }
    
    void Appender::_deleteAllAppenders() {
        threading::ScopedLock lock(_appenderMapMutex);
        AppenderMap& allAppenders = _getAllAppenders();
        for(AppenderMap::iterator i = allAppenders.begin(); i != allAppenders.end(); ) {
            Appender *app = (*i).second;
            i++; // increment iterator before delete or iterator will be invalid.
            delete (app);
        }
    }    

    Appender::Appender(const std::string& name) :
        _name(name) {
        _addAppender(this);
    }
    
    Appender::~Appender() {
        _removeAppender(this);
    }
}
