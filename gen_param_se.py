#! /usr/bin/python
#
# syntax:
# ------------------------------------------------
#     gen_param_se csvdir=csv_files_dir pardir=parameter_files_dir e range=1-2
# example:
#     ./test/run_sim.py --csvdir=./csv 
#                       --pardir=./config_template
#                       --rangelow=1
#                       --rangehigh=2
#
# this module read the simulation parameters based on csv input and generate corresponding .par file
# it performs the following functions:
# ------------------------------------------------
# 1), traverse the csv file
# 2), read in the .par template filel for each physical channel type
# 3), parse each csv line and generate corresponding .par file based on template
# 4), loop step 3) until the last test
#
# ------------------------------------------------


import sys
import os
import re
import csv
import shutil

def fandParlistRange( words):
    global lineListEnd
    global lineList
    global offset
    global parNameList
    global parStartLineNumList
    global parNameStack
    global parStartLineNumStack
   
    for i in range(0, lineListEnd):
        if(offset >= lineListEnd):
            return offset
        if re.search('}', lineList[offset]):
            offset =offset +1
            #print words
            name = ''
            for elem2 in parNameStack:
                name = name + '.' + elem2
            
            parNameList.append(name)
            parNameStack.pop()
            parStartLineNumList.append(parStartLineNumStack.pop())
            
            return offset
        elif re.search('ParamList', lineList[offset]):
            words = re.split(r'\s+', lineList[offset])
            wordsIndex = words.index('ParamList')
            parNameStack.append(words[wordsIndex-1])
            parStartLineNumStack.append(offset)
            offset =offset +1            
            fandParlistRange(words)
        else:
            offset =offset +1
            continue

    return offset

def changePar(start, end, searchstring, replaceString):
    global lineList
    for i in range(start, end):
        if searchstring in lineList[i]:
            lineList[i] = re.sub('(\[.*?\])', replaceString, lineList[i])

pComment = re.compile(r'//.*')

from optparse import OptionParser

print('current working directory:' + os.getcwd())

print 'input parameters:'
print sys.argv

# set up command line options
parser = OptionParser()
# set up syntax prompt
parser.set_usage('''
    gen_param_se --csvdir=csv_files_dir 
                 --pardir=parameter_files_dir 
                 --rangehigh=high 
                 --rangelow=low''')
parser.add_option("-c", "--csvdir",    dest="csvdir",     help="csv file directory",   metavar="csv_file")
parser.add_option("-p", "--pardir",    dest="pardir",     help="param file directory", metavar="parameter_file_directory")
parser.add_option("-m", "--rangehigh", dest="range_high", help="maximum test number",  metavar="max_test_id")
parser.add_option("-l", "--rangelow",  dest="range_low",  help="minimum test number",  metavar="min_test_id")

# grab options
(options, args) = parser.parse_args()
if len(args) != 0:
    parser.error("wrong syntax")

# get csv file name
csvpar = options.csvdir

# get parameter files directory
pardir = options.pardir

# get the test number to run, range [low:high]
rangeLow = options.range_low 
rangeHigh = options.range_high
assert int(rangeHigh) >= int(rangeLow), 'invalid test range'
assert int(rangeLow) >= 0, 'invalid test range low'

# initialize dictionary to index different channel types
ch_dic = {\
          'dch'     :0, \
          'hsscch'  :0, \
          'hspdsch' :0  \
          }

# fetch all .par files under directory csvdir, each storing parameters for certain physical channel type
for csvpar_element in os.listdir(csvpar):
#{
    if not csvpar_element.endswith(".csv"):
        continue
    
    # extract physical channel type from template file with name like config_dch.par
    filename_dec = re.split('_|\.', csvpar_element)
    if (len(filename_dec) != 3):
        continue
    ch_type = filename_dec[1]
    
    csv_file = csv.reader(open(csvpar + os.sep + csvpar_element, 'rb'))
    # initialize test number
    test_number = 0
    for row in csv_file:
    #{
        # line 1 parameter name
        if (csv_file.line_num == 1):
            param_name = row
            continue
        
        if (row[0] == ''):
            # this row is comment row.
            continue
        
        if (int(row[0]) >= int(rangeLow)) and (int(row[0]) <= int(rangeHigh)):
            if (row[0] != '') and (row[0] != test_number):
                # for a new test we need initialize the context variables
                # row[0] is test number
                # row[1] is test directory name, create if not exist
                # row[2] is comments for the tests
                test_number = row[0]
                testDirName = row[1]
                commentsName= row[2]
                # reset channel index in a new test
                ch_dic[ch_type] = 0
            
            ch_index = ch_dic[ch_type]
            print "Read csv file: {}, channel type is {} with index {}".format(csvpar_element, ch_type, ch_index)
        else:
            continue
        
        # create simulation directory if it doesn't exist
        if not os.path.exists(testDirName):
            os.mkdir(testDirName)
        # copy template par file into the test directory
        shutil.copy(pardir+os.sep+'Config_'+ch_type+'.par', testDirName+os.sep)
        
        # open template par file for modification
        inputf  = open(testDirName + os.sep + 'Config_' + ch_type + '.par')
        outputf = open(testDirName + os.sep + 'temp.file', 'w')
        
        # join all lines in the template files with all comments removed
        lineList = []
        for line in inputf :
            str1 = pComment.sub(' ', line)
            lineList.append(str1)
            
        # record the start/end position of each paire of brace recursively.
        # each unit represent one independent parameter class.
        parNameList = []
        parNameStack = []
        parStartLineNumList = []
        parStartLineNumStack = []
        parEndLineNumList = []
        words = ['erro']
        j = 0
        for elem1 in lineList:
            if re.search('}',elem1):
                parEndLineNumList.append(j)
            j += 1
        lineListEnd = j
        offset = 0
        fandParlistRange(words)
        
        # read the parameter line by line and generate config.par accordingly
        # each line contains the description for one physical channel
        ii = -1
        for elem1 in row :
        #{
            ii += 1
            # parameter starts from column 3
            if (ii < 3): 
                continue
            
            SearchNameList = re.split('\.', param_name[ii])
            paramSearchName = SearchNameList.pop()
            paramListSearchName = ''
            if SearchNameList:
                for elem2 in SearchNameList:
                    paramListSearchName  = paramListSearchName  + '.' + elem2
            
            replaceString = '['+(row[ii]).lower()+']'
            
            if paramListSearchName:
                if parNameList.count(paramListSearchName) >= 1:
                    parIndex = parNameList.index(paramListSearchName)
                    changePar(parStartLineNumList[parIndex], parEndLineNumList[parIndex], paramSearchName, replaceString)
            else:
                changePar(0, lineListEnd, paramSearchName, replaceString)
        #} for elem1 in row :
        
        for elem1 in lineList:
            outputf.write(elem1)
        
        inputf.close();
        outputf.close();
        
        # rename par file and increase index of channel accordingly
        os.remove(testDirName + os.sep + 'Config_' + ch_type + '.par')
        os.rename(testDirName + os.sep + 'temp.file', testDirName + os.sep + 'Config_' + ch_type + '_' + str(ch_index) + '.par')
        ch_dic[ch_type] += 1
        
        commentf = open(testDirName+os.sep+'comments.txt', 'w')
        print>>commentf, commentsName
        commentf.close()
        
    #} for row in csv_file:
#} for csvpar_element in os.listdir(csvpar):