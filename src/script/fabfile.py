#from __future__ import with_statement
from fabric.api import *
import datetime

env.use_ssh_config = True
env.disable_known_hosts = True

env.roledefs = {
	'test' : ['test'],
	'live' : ['mit1','mit2']
}

home='~/'

def default_roles(*role_list):
	def selectively_attach(func):
		if not env.roles and not env.hosts:
			return roles(*role_list)(func)
		else:
			if env.host:
				func = hosts(*env.hosts)(func)
			if	env.roles:
				func = roles(*env.roles)(func)
			return func
	return selectively_attach

@default_roles('test')
def restart():
	run('cd %s;./runserver.sh restart' %home,pty=False)

@default_roles('test')
def start():
	run('cd %s;./runserver.sh start' %home, pty=False)

@default_roles('test')
def stop():
	with settings(warn_only=True):
		run('cd %s;./runserver.sh stop' %home, pty=False)

@default_roles('test')
def status():
	with settings(warn_only=True):
		run('cd %s;./runserver.sh status' % home, pty=False)

@default_roles('live')
def df():
		run('df -h | grep \'home\'')

@default_roles('test')
def deploy(version=None):
	if version is None:
		version = datetime.datetime.now().strftime("%Y%m%d%H%M")
	put('./pinocchio','%s/pinocchio_%s' % (home,version))
	run('chmod +x %s/pinocchio_%s' % (home,version))
	run('sed -i \'1a\[pinocchio_%s\]\' %s/pinocchio.conf' %(version,home))
	run('sed -i \'1d\' %s/pinocchio.conf' %(home))
	
	with settings(warn_only=True):
		run('rm -f %s/PINOCCHIO' %home)
	run('ln -s %s/pinocchio_%s %s/PINOCCHIO' % (home,version,home))

@default_roles('test','live')
def version():
	run('cd %s;readlink PINOCCHIO' %home)

@default_roles('test','live')
def deploy_sh():
	put('runserver.sh','%s/runserver.sh' %home)
	run('chmod +x %s/runserver.sh' %home)
