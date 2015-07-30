__author__ = 'joe'
# -*- coding: utf-8 -*-
PRODUCT_INFO='Batch registration tool to add mail user and send short message.\r\n'

#SM profile

import os
if os.name=='nt':
    WMPROFILE='c:/etc/webmail.pf'
    SMPROFILE='c:/etc/smc.pf'
else:
    WMPROFILE='/etc/webmail.pf'
    SMPROFILE='/etc/smc.pf'


MAX_VALID_CODE=999999
MAIL_DELIMITER='\xFF'

EMAIL_MOBILE_NUMBER	    = 'emailmobilenumber'
EMAIL_MOBILE_CODE	    = 'emailmobilecode'
SYS_EMAIL_MOBILE_CODE   = 'emailmobilecodesys'

SMS_NUMBER              = 'SMS_NUMBER'
SMS_EMAIL_NOTIFY        = 'SMS_EMAILNOTIFY'


import os
import sys
import string
import random


import srpc
import smlib
import smcic

IsDigits=smlib.IsDigits

# Logging support
# ===============

logfile = ""            # Filename to log to, if not empty
logfp = None            # File object to log to, if not None

def initlog(*allargs):
    """Write a log message, if there is a log file.

    Even though this function is called initlog(), you should always
    use log(); log is a variable that is set either to initlog
    (initially), to dolog (once the log file has been opened), or to
    nolog (when logging is disabled).

    The first argument is a format string; the remaining arguments (if
    any) are arguments to the % operator, so e.g.
        log("%s: %s", "a", "b")
    will write "a: b" to the log file, followed by a newline.

    If the global logfp is not None, it should be a file object to
    which log data is written.

    If the global logfp is None, the global logfile may be a string
    giving a filename to open, in append mode.  This file should be
    world writable!!!  If the file can't be opened, logging is
    silently disabled (since there is no safe place where we could
    send an error message).

    """
    global logfp, log
    if logfile and not logfp:
        try:
            logfp = open(logfile, "a")
        except IOError:
            pass
    if not logfp:
        log = nolog
    else:
        log = dolog
    apply(log, allargs)

def dolog(fmt, *args):
    """Write a log message to the log file.  See initlog() for docs."""
    logfp.write(fmt%args + "\n")

def nolog(*allargs):
    """Dummy function, assigned to log when logging is disabled."""
    pass

log = initlog           # The current logging function


#Get Mail Server
def getServer():
    addr=smlib.getConfigStr(WMPROFILE,'NET','SERVER','127.0.0.1')
    port=smlib.getConfigInt(WMPROFILE,'NET', 'PORT',9010)
    return (addr,port)

#Get Sms Server
def getSMServer():
    addr=smlib.getConfigStr(SMPROFILE,'setup','ServerIp','127.0.0.1')
    port=smlib.getConfigInt(SMPROFILE,'setup','Port',8801)
    return (addr,port)


def connectServer():
    addr=getServer()
    cli=srpc.SRPCClient()
    if cli.Connect(addr[0],str(addr[1]))==0:
        cli.Close()
        return None
    return cli

def connectSMServer():
    addr=getSMServer()
    cli=srpc.SRPCClient()
    if cli.Connect(addr[0],str(addr[1]))==0:
        cli.Close()
        return None
    return cli


def BatchRegister():

    global logfile, logfp
    print PRODUCT_INFO

    print 'Input file name:'
    st=sys.stdin
    file=st.readline()
    #print file

    file=string.strip(file)
    if not os.path.exists( file) or not os.path.isfile( file):
        print '%s is directory or does not exist.\r\n' % file
        return

    try:
        fd=open(file,'r')
    except IOError, err:
        print err
        print
        return

    head,tail=os.path.splitext(file)
    logfile=head+'.err'
    domain=smlib.getConfigStr(WMPROFILE,'domain','domain','zj165.com')


    mail_clnt=connectServer()
    if not isinstance(mail_clnt,srpc.SRPCClient):
        fd.close()
        print "Can't connect to mail server!\r\n"
        return

    sms_clnt=connectSMServer()
    if not isinstance(sms_clnt,srpc.SRPCClient):
        fd.close()
        mail_clnt.Close()
        print "Can't connect to sms server!\r\n"
        return

    ok=0
    all=0
    false=0
    while(1):
        line=fd.readline()
        if not line:
            break

        line=string.strip(line)
        if len(line)==0:
            continue

        print line
        if not IsDigits(line):
            all+=1
            false+=1
            log( '%s is not digits\n', line)
            continue

        rand="%06d" % random.randint(0,MAX_VALID_CODE)
        userdef="%s%s%s" % ( line, MAIL_DELIMITER, rand)

        retcode=smcic.call_add_user(mail_clnt,line,domain,userdef)
        if len(retcode)!=1 or retcode[0]!=1 :
            all+=1
            false+=1
            log( 'add mail user %s@%s fail, return %s !', line,domain,retcode)
            continue

        retcode=smcic.call_find_user(mail_clnt,line,domain)
        if len(retcode)!=2 or retcode[0]!=1 :
            log( 'find mail user %s@%s fail, return %s !', line,domain,retcode)

        else:
            uid=retcode[1]
            smcic.call_set_userval( mail_clnt, uid, EMAIL_MOBILE_NUMBER, line)
            smcic.call_set_userval( mail_clnt, uid, EMAIL_MOBILE_CODE, '1')
            smcic.call_set_userval( mail_clnt, uid, SYS_EMAIL_MOBILE_CODE, '1')
            smcic.call_set_userval( mail_clnt, uid, SMS_NUMBER, line)
            smcic.call_set_userval( mail_clnt, uid, SMS_EMAIL_NOTIFY, '1')


        msg='尊敬的用户,您的联通如意邮箱 %s@%s已经开通,将免费试用至5月31日,密码为 %s,详情请登录 mail.zj165.com.' % (line,domain,rand)
        retcode=smlib.SendSingle(sms_clnt, line, msg)
        if len(retcode)<1 or retcode[0]!=1 :
            all+=1
            false+=1
            log( 'mail user %s@%s added, but send short message to %s fail, return %s !', line,domain,line,retcode)
            continue

        all+=1
        ok+=1

    fd.close()
    if logfp:
        logfp.close()

    mail_clnt.Close()
    sms_clnt.Close()

    print '%d mobile number processed.' % all
    print '%d mail user added, short message sent' % ok

    if false>0:
        print '%d mobile number process failed, error has been writen to %s.' %( false, logfile)


if __name__=='__main__':
    BatchRegister()
    print
    print 'Press any key to exit...'

    raw_input()