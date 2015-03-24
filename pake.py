#!/usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2005-2010; Arne Babenhauserheide

"""Waffle iron - creates waffles :)

- http://draketo.de/proj/waffles/

TODO: Differenciate sourcetree and packages: sourcetree as dir in wafdir, packages in subdir packages. 
"""

__license__ = """
This license only applies to the waffle part of the code,
which ends with
### Waffle Finished ###

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
"""

import os, sys, optparse, getpass, re, binascii
if sys.hexversion<0x204000f: raise ImportError("Waffle requires Python >= 2.4")
try:
	from hashlib import md5
except:
	from md5 import md5 # for python < 2.5

if 'PSYCOWAF' in os.environ:
	try:import psyco;psyco.full()
	except:pass

VERSION="0.1"
REVISION="55399a875cc742b0c987288ee2339fe8"
INSTALL=''
cwd = os.getcwd()
join = os.path.join
HOME = os.path.expanduser('~')

WAF='waffle' #: the default name of the executable
WAFFLE='waffle' #: the default name of the dir with the sources (prepended with s when unpacked)
WAFFLE_MAKER='waffle_maker.py'

def parse_cmdline_args():
	"""@return: opts, args; opts are parsed"""
	# parse commandline arguments.
	parser = optparse.OptionParser()
	parser.add_option("-o", "--filename", 
			  help="Set the output filename", default="waffle.py", metavar="OUTPUT_FILE")
	parser.add_option("-p", "--package", action="append", dest="packages",
		  help="Package folder to include (can be used multiple times)", metavar="PACKAGE_FOLDER")
	parser.add_option("-m", "--module", action="append", dest="modules", 
			  help="Python module to include (can be used multiple times)", metavar="module_to_include.py")
	parser.add_option("-s", "--script", 
			  help="Execute this script", default="run.py", metavar="script_to_run.py")
	parser.add_option("--unpack-only", action="store_true", 
			  help="only unpack the tar.bz2 data, but don't execute anything.", default=False)
	opts, args = parser.parse_args()
	if opts.modules is None:
		opts.modules = []
	if opts.packages is None:
		opts.packages = []
			      
	return opts, args

def b(x):
	return x

if sys.hexversion>0x300000f:
	WAF='waffle3'
	def b(x):
		return x.encode()

def err(m):
	print(('\033[91mError: %s\033[0m' % m))
	sys.exit(1)

def get_waffle_data():
	f = open(sys.argv[0],'r')
	c = "corrupted waf (%d)"
	while True:
		line = f.readline()
		if not line: err("no data")
		if line.startswith('#==>'):
			txt = f.readline()
			if not txt: err("wrong data: data-line missing")
			if not f.readline().startswith('#<=='): err("wrong data: closing line missing")
			return txt

def unpack_wafdir(txt, zip_type="bz2"):
	"""@param txt: The compressed data"""
	if not txt: err(c % 3)
	if sys.hexversion>0x300000f:
		txt = binascii.a2b_base64(eval("b'" + txt[1:-1] + r"\n'"))
	else: 
		txt = binascii.a2b_base64(txt[1:])

	# select the target folder
	import shutil, tarfile

	s = '.%s-%s-%s'
	if sys.platform == 'win32': s = s[1:]

	## Firstoff we we select some possible folders, to be tested one after the other (with the appropriate precautions).
	## For the sake of readability we first note the different options here.
	#: The home folder as the best option (if the user has a writeable home)
	dirhome = join(HOME, s % (WAF, VERSION, REVISION))
	# the scripts dir
	name = sys.argv[0]
	base = os.path.dirname(os.path.abspath(name))
	#: As second option use the folder where the script resides (if writeable by us - and not yet taken, which could be, if another user started the script). 
	dirbase = join(base, s % (WAF, VERSION, REVISION), getpass.getuser())
	#: tmp as last resort
	dirtmp = join("/tmp", getpass.getuser(), "%s-%s-%s" % (WAF, VERSION, REVISION))

	def prepare_dir(d):
		"""create the needed folder"""
		os.makedirs(join(d, WAFFLE))

	def check_base(d):
		"""Check the dir in which the script resides.

		Only use the dir, if it belongs to us. If we can’t trust the scripts dir, we’re fragged anyway (someone could just tamper directly with the script itself - or rather: could compromise anything we run)."""
		prepare_dir(d)
		return d

	def check_tmp(d):
		"""Check the tmp dir - always remove the dir before startup.

		This kills the caching advantage, but is necessary for security reasons (else someone could create a compromised dir in tmp and chmod it to us)."""
		# last resort: tmp
		if os.path.exists(d):
			try: shutil.rmtree(d)
			except OSError: err("Can't remove the previously existing version in /tmp - executing would endanger your system")
			try: 
				prepare_dir(d)
				return d
			except OSError: err("Cannot unpack waf lib into %s\nMove waf into a writeable directory" % dir)

	## Now check them. 
	# first check: home
	try:
		d = dirhome
		prepare_dir(d)
	except OSError:
		# second check: base
		if base.startswith(HOME) or sys.platform == 'win32':
			try:
				d = check_base(dirbase)
			except OSError:
				d = check_tmp(dirtmp)
		else: d = check_tmp(dirtmp)

	## Now unpack the tar.bz2 stream into the chosen dir. 
	os.chdir(d)
	if zip_type == 'bz2': 
		tmp = 't.tbz2'
	elif zip_type == 'gz':
		tmp = 't.gz'
	t = open(tmp,'wb')
	t.write(txt)
	t.close()

	try:
		t = tarfile.open(tmp)
	# watch out for python versions without bzip2
	except:
		try: 
			os.system('bunzip2 t.bz2')
			t = tarfile.open('t')
		except:
			# if it doesn’t work, go back and remove the garbage we created.
			try: 
				os.unlink(tmp)
			except OSError: pass
			os.chdir(cwd)
			try: shutil.rmtree(d)
			except OSError: pass
			err("Waf cannot be unpacked, check that bzip2 support is present")

	for x in t:
		t.extract(x)
	t.close()
	os.unlink(tmp)


	#if sys.hexversion>0x300000f:
		#sys.path = [join(d, WAFFLE)] + sys.path
		#import py3kfixes
		#py3kfixes.fixdir(d)

	os.chdir(cwd)
	return join(d, WAFFLE)


def make_waffle(base_script="waffle_maker.py", packages=[], modules=[], folder=WAFFLE, executable="run.py", target="waffle.py", zip_type="bz2"):
	"""Create a waf-like waffle from the base_script (make_waffle.py), the folder and a python executable (appended to the end of the waf-light part)."""
	print("-> preparing waffle")
	mw = 'tmp-waf-'+VERSION

	import tarfile, re, shutil

	if zip_type not in ['bz2', 'gz']:
		zip_type = 'bz2'

	# copy all modules and packages into the build folder
	if not os.path.isdir(folder):
		os.makedirs(folder)
	
	for i in modules + packages:
		if i.endswith(os.path.sep): 
			i = i[:-1]
		if os.path.isdir(i) and not os.path.isdir(join(folder, i.split(os.path.sep)[-1])):
			shutil.copytree(i, join(folder, i.split(os.path.sep)[-1]))
		elif os.path.isfile(i): 
			shutil.copy(i, folder)

	#open a file as tar.[extension] for writing
	tar = tarfile.open('%s.tar.%s' % (mw, zip_type), "w:%s" % zip_type)
	tarFiles=[]

	def all_files_in(folder): 
		"""Get all paths of files inside the folder."""
		filepaths = []
		walked = [i for i in os.walk(folder)]
		for base, dirs, files in walked:
		    filepaths.extend([os.path.join(base, f) for f in files])
		return filepaths
		
	files = [f for f in all_files_in(folder) if not f.endswith(".pyc") and not f.endswith(".pyo") and not "/." in f]

	for x in files:
		tar.add(x)
	tar.close()

	# first get the basic script which sets up the path
	f = open(base_script, 'r')
	code1 = f.read()
	f.close()
	# make sure it doesn't do anything.
	code1.replace("__name__ == '__main__':", "__name__ == '__main__' and False:")
	# then append the code from the executable 
	if executable is not None:
		f = open(executable, 'r')
		code1 += f.read()
		f.close()

	# now store the revision unique number in waf
	#compute_revision()
	#reg = re.compile('^REVISION=(.*)', re.M)
	#code1 = reg.sub(r'REVISION="%s"' % REVISION, code1)

	prefix = ''
	#if Build.bld:
	#	prefix = Build.bld.env['PREFIX'] or ''

	reg = re.compile('^INSTALL=(.*)', re.M)
	code1 = reg.sub(r'INSTALL=%r' % prefix, code1)
	#change the tarfile extension in the waf script
	reg = re.compile('bz2', re.M)
	code1 = reg.sub(zip_type, code1)

	f = open('%s.tar.%s' % (mw, zip_type), 'rb')
	cnt = f.read()
	f.close()

	# the REVISION value is the md5 sum of the binary blob (facilitate audits)
	m = md5()
	m.update(cnt)
	REVISION = m.hexdigest()
	reg = re.compile('^REVISION=(.*)', re.M)
	code1 = reg.sub(r'REVISION="%s"' % REVISION, code1)
	f = open(target, 'w')
	f.write(code1)
	f.write('#==>\n')
	data = str(binascii.b2a_base64(cnt))
	if sys.hexversion>0x300000f:
		data = data[2:-3] + '\n'
	f.write("#"+data)
	f.write('#<==\n')
	f.close()

	# on windows we want a bat file for starting.
	if sys.platform == 'win32':
		f = open(target + '.bat', 'wb')
		f.write('@python -x %~dp0'+target+' %* & exit /b\n')
		f.close()

	# Now make the script executable
	if sys.platform != 'win32':
		# octal prefix changed in 3.x from 0xxx to 0oxxx. 
		if sys.hexversion>0x300000f:
			os.chmod(target, eval("0o755"))
		else:
			os.chmod(target, eval("0755"))

	# and get rid of the temporary files
	os.unlink('%s.tar.%s' % (mw, zip_type))
	shutil.rmtree(WAFFLE)
	

def test(d):
	try: 
	      os.stat(d)
	      return os.path.abspath(d)
	except OSError: pass

def find_lib():
	"""Find the folder with the modules and packages.

	@return: path to to folder."""
	name = sys.argv[0]
	base = os.path.dirname(os.path.abspath(name))

	#devs use $WAFDIR
	w=test(os.environ.get('WAFDIR', ''))
	if w: return w

	#waffle_maker.py is executed in place.
	if name.endswith(WAFFLE_MAKER):
		w = test(join(base, WAFFLE))
		# if we don’t yet have a waffle dir, just create it.
		if not w:
			os.makedirs(join(base, WAFFLE))
			w = test(join(base, WAFFLE))
		if w: return w
		err("waffle.py requires " + WAFFLE + " -> export WAFDIR=/folder")

	d = "/lib/%s-%s-%s/" % (WAF, VERSION, REVISION)
	for i in [INSTALL,'/usr','/usr/local','/opt']:
		w = test(i+d)
		if w: return w

	# first check if we can use HOME/s,
	# if not, check for s (allowed?)
	# then for /tmp/s (delete it, if it already exists,
	# else it could be used to smuggle in malicious code)
	# and finally give up. 
	
	#waf-local
	s = '.%s-%s-%s'
	if sys.platform == 'win32': s = s[1:]
	# in home
	d = join(HOME, s % (WAF, VERSION, REVISION), WAFFLE)
	w = test(d)
	if w: return w

	# in base
	if base.startswith(HOME):
		d = join(base, s % (WAF, VERSION, REVISION), WAFFLE)
		w = test(d)
		if w: return w
	# if we get here, we didn't find it.
	return None


wafdir = find_lib()
if wafdir is None: # no existing found
	txt = get_waffle_data() # from this file
	if txt is None and __name__ == "__main__": # no waffle data in file
		opts, args = parse_cmdline_args()
		make_waffle(packages=opts.packages, modules=opts.modules, executable=opts.script)
	else: 
		wafdir = unpack_wafdir(txt)
	
elif sys.argv[0].endswith(WAFFLE_MAKER) and __name__ == "__main__": # the build script called
	opts, args = parse_cmdline_args()
	if opts.filename.endswith(WAFFLE_MAKER):
		err("Creating a script whose name ends with " + WAFFLE_MAKER + " would confuse the build script. If you really want to name your script *" + WAFFLE_MAKER + " you need to adapt the WAFFLE_MAKER constant in " + WAFFLE_MAKER + " and rename " + WAFFLE_MAKER + " to that name.")
	make_waffle(packages=opts.packages, modules=opts.modules, executable=opts.script, target=opts.filename)
	# since we’re running the waffle_maker, we can stop here. 
	exit(0)

if wafdir is not None: 
	sys.path = [wafdir] + [join(wafdir, d) for d in os.listdir(wafdir)] + sys.path


## If called with --unpack-only, no further code is executed.
if "--unpack-only" in sys.argv:
	print(sys.argv[0], "unpacked to", wafdir)
	exit(0)

### Waffle Finished ###

#!/usr/bin/env python

import fsutils
import ui
import targets
import variables
import configurations
import parser

def parse_source_tree():
    for filename in fsutils.pake_files:
        parser.parse(filename)

    configuration = configurations.get_selected_configuration()
    variables.export_special_variables(configuration)

def main():
    import command_line

    parse_source_tree()

    configuration = configurations.get_selected_configuration()
    if configuration.name != "__default":
        ui.bigstep("configuration", str(configurations.get_selected_configuration()))

    if command_line.args.target:
        for target in command_line.args.target:
            targets.build(target)
    elif command_line.args.all:
        targets.build_all()
    else:
        ui.info("no target selected\n")

        ui.info(ui.BOLD + "targets:" + ui.RESET)
        for target in targets.targets.values():
            ui.info("  " + str(target))

        ui.info(ui.BOLD + "\nconfigurations:" + ui.RESET)
        for configuration in configurations.configurations:
            ui.info("  " + str(configuration))

        ui.info("\nsee --help for more\n")

if __name__ == '__main__':
    main()

#==>
#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
#<==
