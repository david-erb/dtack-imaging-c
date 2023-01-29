@echo on
@rem those with no defaults can go in evironment variables
@set DT_ERRTO=out
@set DT_DBGTO=out
@set DT_SAYTO=out
@set DT_D=alimits+complex+object+usr2+time
static\dttfb 0 0 0 -D time+ipc -
@rem more out
@rem dtutil tail out `
:exit
@echo k.bat: exiting
