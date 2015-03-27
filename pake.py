#!/usr/bin/env python2
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
REVISION="ad96d19189466fd6327e096470f7c3f7"
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
