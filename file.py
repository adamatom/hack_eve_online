import xtriui
import form
import uix
import util
import listentry
import uthread
import draw
import base
import service
import time
import dbutil
import blue
import util
import listentry
import uthread
import base
import service
import stackless
import sys
import blue
import uthread
import uix
import xtriui
import form
import draw

def MessageBox(text, title = "EVE", buttons = None, icon = None, suppText = None, customicon = None, height = None, blockconfirmonreturn = 0, default = None, modal = True):
	msgbox = eve.LocalSvc("window").GetWindow("adamadam", create=1, prefsName="modal", ignoreCurrent=1)


def DumpOrdersForType(typeID):
	orders = eve.LocalSvc("marketQuote").GetOrders(typeID)
	sells = orders[0][0]
	buys = orders[1][0]
	if (len(sells) > 0):
		dateIdx = sells[0].__keys__.index("issued")
	elif (len(buys) > 0):
		dateIdx = buys[0].__keys__.index("issued")
	else:
		return
	date = util.FmtDate(blue.os.GetTime())
	f = blue.os.CreateInstance("blue.ResFile")
	typeName = cfg.invtypes.Get(typeID).name
	invalidChars = '\\/:*?"<>|'
	for i in invalidChars:
		typeName = typeName.replace(i, "")

	#directory = (blue.win32.SHGetFolderPath(blue.win32.CSIDL_PERSONAL) + "\\EVE\\logs\\Marketlogs\\")
	directory = "C:\\ehack\\")
	filename = ("%s-%s-%s.txt" % (cfg.evelocations.Get(session.regionid).name,
	 typeName,
	 util.FmtDate(blue.os.GetTime()).replace(":", "")))
	if (not f.Open((directory + filename), 0)):
		f.Create((directory + filename))
	first = 1
	for order in sells:
		if first:
			for key in order.__keys__:
				f.Write(("%s," % key))

			f.Write("\r\n")
			first = 0
		for i in range(len(order)):
			first = 0
			if (i == dateIdx):
				f.Write(("%s," % util.FmtDate(order[i], "en")))
			else:
				f.Write(("%s," % order[i]))

		f.Write("\r\n")

	for order in buys:
		if first:
			for key in order.__keys__:
				f.Write(("%s," % key))

			f.Write("\r\n")
			first = 0
		for i in range(len(order)):
			if (i == dateIdx):
				f.Write(("%s," % util.FmtDate(order[i], "en")))
			else:
				f.Write(("%s," % order[i]))

		f.Write("\r\n")

	f.Close()
	eve.Message("MarketExportInfo", {"sell": len(sells),
	 "buy": len(buys),
	 "typename": cfg.invtypes.Get(typeID).name,
	 "filename": (("<b>" + filename) + "</b>"),
	 "directory": ("<a href=shellexec:%s>%s</a>" % (directory.replace("\\", "/").replace(" ", "%20"),
				   directory))})

def adam():
	try:
	
		while 1:
			print "Running!"
			blue.pyos.synchro.Sleep(10000)


			DumpOrdersForType(37)




			history = eve.LocalSvc("marketQuote").GetPriceHistory(11824)
			date = util.FmtDate(blue.os.GetTime())
			f = blue.os.CreateInstance("blue.ResFile")
			typeName = cfg.invtypes.Get(11824).name
			invalidChars = '\\/:*?"<>|'
			for i in invalidChars:
				typeName = typeName.replace(i, "")

			directory = (blue.win32.SHGetFolderPath(blue.win32.CSIDL_PERSONAL) + "\\EVE\\logs\\Marketlogs\\")
			filename = ("%s-%s-%s-HISTORY.txt" % (cfg.evelocations.Get(session.regionid).name,
			 typeName,
			 util.FmtDate(blue.os.GetTime()).replace(":", "")))

			if (not f.Open((directory + filename), 0)):
				f.Create((directory + filename))
			skipcount = 0

			f.Write("Date,Orders,Volume,Low,High,Average\r\n")
			for rec in history:
				if(skipcount > 0):
					f.Write(("%s," % util.FmtDate(rec[0],"en")))
					f.Write(("%s," % rec[5]))
					f.Write(("%s," % rec[4]))
					f.Write(("%s," % rec[1]))
					f.Write(("%s," % rec[2]))
					f.Write(("%s" % rec[3]))
					f.Write("\r\n")
				else:
					skipcount = skipcount + 1


	finally:
		print "adam is dieing"
		
		



uthread.pool("adam", adam)



