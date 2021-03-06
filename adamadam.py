
import blue 
import uix 
import xtriui 
import types 
import uix 
import draw 
import uthread



 
class adamadam(xtriui.DockWindow):
    __guid__ = "form.adamadam"

    def Startup(self, *args):
        self.name = "adamadam"
        self.scope = "all"
        self.SetCaption(mls.UI_GENERIC_CALCULATOR)
        self.MakeUnResizeable()
        self.SetWndIcon("49_01")
        self.HideMainIcon()
        self.SetTopparentHeight(21)
        self.sr.main = uix.GetChild(self, "main")
        draw.Container("psuh", parent=self.sr.topParent, align=uix.UI_ALLEFT, width=uix.Border(), idx=0)
        iconPar = draw.Container("iconPar", parent=self.sr.topParent, align=uix.UI_ALRIGHT, width=18, idx=0)
        icon = xtriui.MiniButton(uix.GetMappedIcon("38_156", iconPar, 0, 0, 2, 16, align=uix.UI_ALNONE, scaling=0.0625))
        icon.groupID = "viewMode"
        self.sr.viewModeBtn = icon
        icon.Click = self.ViewModeBtnClick
        inpt = uix.GetEdit("inpt", self.sr.topParent, align=uix.UI_ALTOP)
        inpt.OnAnyChar = self.OnInptChar
        self.sr.inpt = inpt
        self.SetInpt("0")
        hist = settings.ui.Get("CalculatorHistory", [])
        if hist:
            ops = [ (v,
             v) for v in hist ]
        else:
            ops = [('0', '0')]
        inpt.LoadCombo("urlcombo", ops, self.OnComboChange)
        self.sr.mem = uix.GetContainer("mem", where=self.sr.main, left=uix.Border(), top=3, height=16, align=uix.UI_ALTOP)
        self.sr.work = uix.GetContainer("work", where=self.sr.main, left=uix.Border(), align=uix.UI_ALCLIENT)
        size = 24
        for (x, y, label, cap,) in self.buttons:
            btn = uix.GetBigButton(size=size, where=self.sr.work, left=int((x * size)), top=int((y * size)), iconMargin=10, align=uix.UI_ALNONE)
            btn.SetInCaption(cap)
            btn.Click = self.OnBtnClick
            btn.name = label
            setattr(self.sr, ("btn%s" % label), btn)

        self.SetMinSize([((btn.left + size) + 8),
         ((btn.top + size) + 60)], 1)
        draw.Line(self.sr.mem, uix.UI_ALTOP)
        draw.Line(self.sr.mem, uix.UI_ALBOTTOM)
        for i in xrange(1, 7):
            opt = xtriui.StandardMenu(uix.GetContainer("memoption", where=self.sr.mem, align=uix.UI_ALLEFT, pickable=1))
            opt.Setup(("<b>M%s</b>" % i), getattr(self, ("GetMem%sMenu" % i), None))
            opt.OnClick = self.OnClickMemory
            opt.sr.memHilite = draw.Fill(opt, align=uix.UI_ALCLIENT, color=(0.0, 0.0, 1.0, 0.20000000000000001), state=uix.UI_HIDDEN, getInstance=0)
            opt.name = ("mem%s" % i)
            opt.label = settings.ui.Get(("CalculatorMem%sName" % i), ("Memory %d" % i))
            if (i == 1):
                opt.left = uix.Border()
            setattr(self.sr, ("memBtn%s" % i), opt)
            opt.mem = settings.ui.Get(("CalculatorMem%s" % i), None)
            opt.nr = i
            if opt.mem:
                opt.hint = ("%s:<br>%s" % (opt.label,
                 opt.mem))
                opt.sr.memHilite.state = uix.UI_DISABLED
            else:
                opt.hint = ("%s:<br>%s" % (opt.label,
                 mls.UI_GENERIC_EMPTY))
                opt.sr.memHilite.state = uix.UI_HIDDEN

        self.SetLayout()
        self.digitSign = prefs.GetValue("digit", ".")
        self.decimalSign = prefs.GetValue("decimal", ".")
        self.newNumber = True
        self.stack = []
        self.opStack = []
        self.lastOp = 0



    def SetLayout(self):
        expanded = settings.ui.Get("CalculatorExpanded", 1)
        if expanded:
            self.SetMinSize([165,
             155], 1)
            h = uix.UI_NORMAL
            self.sr.viewModeBtn.hint = mls.UI_INFLIGHT_HIDEBUTTONS
        else:
            self.SetMinSize([165,
             60], 1)
            h = uix.UI_HIDDEN
            self.sr.viewModeBtn.hint = mls.UI_INFLIGHT_SHOWBUTTONS
        for (x, y, label, cap,) in self.buttons:
            btn = getattr(self.sr, ("btn%s" % label), None)
            btn.state = h




    def MouseDown(self, *args):
        eve.LocalSvc("ui").SetFocus(self.sr.inpt)



    def SetInpt(self, value, new = True):
        self.sr.inpt.SetValue(value)
        self.newNumber = new



    def OnComboChange(self, combo, header, value, *args):
        self.SetInpt(value, False)



    def OnInptChar(self, char, *args):
        _char = char
        text = self.sr.inpt.GetText()
        try:
            _char = unichr(char)
            if ((len(_char) == 1) and (_char in "0123456789")):
                if ((text == "0") or self.newNumber):
                    self.sr.inpt.text = ""
                self.sr.inpt.Insert(char)
                self.newNumber = False
                return False
        except:
            sys.exc_clear()
        if (_char in self.knownkeys.keys()):
            _char = self.knownkeys[_char]
        if (_char == "dot"):
            if ((self.decimalSign not in text) or self.newNumber):
                if ((text == "") or self.newNumber):
                    self.sr.inpt.text = "0"
                    self.sr.inpt.Insert(ord(self.decimalSign))
                    self.newNumber = False
                    return False
                self.sr.inpt.Insert(ord(self.decimalSign))
                self.newNumber = False
            else:
                return False
        if (_char == "plusminus"):
            if text.startswith("-"):
                text = text[1:]
            else:
                text = ("-" + text)
            self.SetInpt(text, False)
            return False
        if (_char == "clearall"):
            self.stack = []
            self.opStack = []
        if (_char in ('\x08', 'clear', 'clearall')):
            self.SetInpt("0")
            return False
        if (_char in self.prio.keys()):
            self.Operator(_char, text)
            if (_char == "equal"):
                hist = settings.ui.Get("CalculatorHistory", [])
                hist.insert(0, text)
                if (len(hist) > 20):
                    hist.pop()
                settings.ui.Set("CalculatorHistory", hist)
                ops = [ (v,
                 v) for v in hist ]
                self.sr.inpt.LoadCombo("urlcombo", ops, self.OnComboChange, setvalue=text)
        return False



    def OnBtnClick(self, btn, *args):
        print "btn.name",
        print btn.name
        try:
            char = ord(btn.name)
        except:
            char = btn.name
            sys.exc_clear()
        self.OnInptChar(char)



    def ViewModeBtnClick(self):
        expanded = settings.ui.Get("CalculatorExpanded", 1)
        expanded ^= 1
        settings.ui.Set("CalculatorExpanded", expanded)
        self.SetLayout()



    def OnClickMemory(self, btn, *args):
        if (getattr(btn, "mem", None) is not None):
            self.SetInpt(str(btn.mem), False)



    def GetMem1Menu(self):
        return self.GetMemMenu(self.sr.memBtn1)



    def GetMem2Menu(self):
        return self.GetMemMenu(self.sr.memBtn2)



    def GetMem3Menu(self):
        return self.GetMemMenu(self.sr.memBtn3)



    def GetMem4Menu(self):
        return self.GetMemMenu(self.sr.memBtn4)



    def GetMem5Menu(self):
        return self.GetMemMenu(self.sr.memBtn5)



    def GetMem6Menu(self):
        return self.GetMemMenu(self.sr.memBtn6)



    def GetMem7Menu(self):
        return self.GetMemMenu(self.sr.memBtn7)



    def GetMem8Menu(self):
        return self.GetMemMenu(self.sr.memBtn8)



    def GetMemMenu(self, btn):
        m = []
        m.append((mls.UI_CMD_SET,
         self.MemDo,
         (btn,
          "Set")))
        if (getattr(btn, "mem", None) is not None):
            m.append((mls.UI_CMD_ADD,
             self.MemDo,
             (btn,
              "Add")))
            m.append((mls.UI_CMD_SUB,
             self.MemDo,
             (btn,
              "Sub")))
            m.append((mls.UI_CMD_CLEAR,
             self.MemDo,
             (btn,
              "Clear")))
        m.append((mls.UI_CMD_ANNOTATE,
         self.MemDo,
         (btn,
          "Name")))
        return m



    def MemDo(self, mem, command):
        if (command == "Set"):
            setattr(mem, "mem", float(self.sr.inpt.GetValue()))
            mem.sr.memHilite.state = uix.UI_DISABLED
        elif (command == "Add"):
            setattr(mem, "mem", (getattr(mem, "mem", 0.0) + float(self.sr.inpt.GetValue())))
        elif (command == "Sub"):
            setattr(mem, "mem", (getattr(mem, "mem", 0.0) - float(self.sr.inpt.GetValue())))
        elif (command == "Clear"):
            setattr(mem, "mem", None)
            mem.sr.memHilite.state = uix.UI_HIDDEN
        elif (command == "Name"):
            format = [{"type": "edit",
              "setvalue": (getattr(mem, "label", "") or ""),
              "labelwidth": 48,
              "label": mls.UI_GENERIC_NAME,
              "key": "name",
              "maxlength": 16,
              "setfocus": 1}]
            retval = uix.HybridWnd(format, mls.UI_CMD_ANNOTATE, icon=uix.QUESTION, minW=300)
            if retval:
                mem.label = retval["name"]
                settings.ui.Set(("CalculatorMem%sName" % mem.nr), mem.label)
        if (getattr(mem, "mem", None) is not None):
            mem.hint = ("%s<br>%.14G" % (mem.label,
             getattr(mem, "mem", 0.0)))
        else:
            mem.hint = ("%s<br>%s" % (mem.label,
             mls.UI_GENERIC_EMPTY))
        settings.ui.Set(("CalculatorMem%s" % mem.nr), getattr(mem, "mem", None))



    def CheckOperator(self):
        if (str(self.stack[-1]) in self.prio.keys()):
            return True
        return False



    def Calc(self):
        if ((len(self.stack) > 0) and self.CheckOperator()):
            op = self.stack.pop()
            if (op in ('equal', 'bo', 'bc')):
                self.Calc()
            if (op == "plus"):
                self.Calc()
                op1 = self.stack.pop()
                self.Calc()
                op2 = self.stack.pop()
                self.stack.append((op2 + op1))
            if (op == "minus"):
                self.Calc()
                op1 = self.stack.pop()
                self.Calc()
                op2 = self.stack.pop()
                self.stack.append((op2 - op1))
            if (op == "times"):
                self.Calc()
                op1 = self.stack.pop()
                self.Calc()
                op2 = self.stack.pop()
                self.stack.append((op2 * op1))
            if (op == "divide"):
                self.Calc()
                op1 = self.stack.pop()
                self.Calc()
                op2 = self.stack.pop()
                try:
                    self.stack.append((op2 / op1))
                except ZeroDivisionError,e:
                    sys.exc_clear()
                    eve.Message("uiwarning03")
                    self.stack.append(0.0)
            if (op == "square"):
                self.Calc()
                op1 = self.stack.pop()
                try:
                    self.stack.append((op1 ** 2.0))
                except OverflowError,e:
                    sys.exc_clear()
                    eve.Message("uiwarning03")
                    self.stack.append(0.0)
            if (op == "root"):
                self.Calc()
                op1 = self.stack.pop()
                try:
                    self.stack.append((op1 ** 0.5))
                except ValueError,e:
                    sys.exc_clear()
                    eve.Message("uiwarning03")
                    self.stack.append(0.0)
            if (op == "percent"):
                self.Calc()
                op1 = self.stack.pop()
                if len(self.stack):
                    op2 = self.stack[-1]
                else:
                    eve.Message("uiwarning03")
                    op2 = 0.0
                self.stack.append(((op2 * op1) / 100.0))



    def Operator(self, op, number):
        if ((not self.newNumber) or (len(self.stack) == 0)):
            self.stack.append(float(number.replace(self.decimalSign, ".")))
        elif ((self.lastOp in ('plus', 'minus', 'times', 'divide')) and (op != "bo")):
            self.opStack[-1] = op
            return
        if ((op != "bo") and ((len(self.opStack) == 0) or (self.prio[op] <= self.prio[self.opStack[-1]]))):
            while ((len(self.opStack) > 0) and (self.prio[op] <= self.prio[self.opStack[-1]])):
                self.stack.append(self.opStack.pop())

        if (op == "bc"):
            while ((len(self.opStack) > 0) and (self.opStack[-1] != "bo")):
                self.stack.append(self.opStack.pop())

            if len(self.opStack):
                self.opStack.pop()
        if (op in ('percent', 'root', 'square', 'equal', 'bc')):
            self.stack.append(op)
        else:
            self.opStack.append(op)
        self.Calc()
        number = ("%.14G" % self.stack[-1])
        self.SetInpt(number.replace(".", self.decimalSign))
        self.lastOp = op


    buttons = [(0, 0, '7', '7'),
     (1, 0, '8', '8'),
     (2, 0, '9', '9'),
     (3, 0, 'divide', '&divide;'),
     (0, 1, '4', '4'),
     (1, 1, '5', '5'),
     (2, 1, '6', '6'),
     (3, 1, 'times', '&times;'),
     (0, 2, '1', '1'),
     (1, 2, '2', '2'),
     (2, 2, '3', '3'),
     (3, 2, 'minus', '-'),
     (0, 3, '0', '0'),
     (1, 3, 'dot', '.'),
     (2, 3, 'equal', '='),
     (3, 3, 'plus', '+'),
     (4.5, 0, 'clear', 'C'),
     (5.5, 0, 'clearall', 'AC'),
     (4.5, 1, 'bo', '('),
     (5.5, 1, 'bc', ')'),
     (4.5, 2, 'percent', '%'),
     (5.5, 2, 'square', 'x&sup2;'),
     (4.5, 3, 'plusminus', '&plusmn;'),
     (5.5, 3, 'root', u'\u221a&macr;')]
    knownkeys = {"+": "plus",
     "-": "minus",
     "*": "times",
     "/": "divide",
     prefs.GetValue("decimal", "."): "dot",
     "=": "equal",
     "r": "root",
     "s": "square",
     "p": "percent",
     "m": "plusminus",
     "(": "bo",
     ")": "bc",
     "C": "clearall",
     "\r": "equal"}
    prio = {"plus": 3,
     "minus": 3,
     "times": 4,
     "divide": 4,
     "square": 5,
     "root": 5,
     "bo": 1,
     "bc": 6,
     "percent": 4,
     "equal": 0}

            
            
            
print "Ran adamadam"







