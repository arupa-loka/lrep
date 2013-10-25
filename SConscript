import os

env = Environment(CC='g++',
				  CCFLAGS='-g -O0')
#				  CCFLAGS='-O3')
print "     CC:", env['CC']
print "CCFLAGS:", env['CCFLAGS']
Export('env')

SetOption('warn','no-all')

env.Repository('datastructures')
env.Repository('algorithms')
env.Repository('readers')
env.Repository('random_generator')

#env = Environment(ENV = {'PATH' : os.environ['PATH'],
#                         'TERM' : os.environ['TERM'],
#                         'HOME' : os.environ['HOME']})


#SConscript('datastructures/SConscript', export=['env'], variant_dir='build' );
SConscript('datastructures/SConscript', export=['env'] );
SConscript('algorithms/SConscript', export=['env'] );
SConscript('readers/SConscript', export=['env'] );

# Print all object files
#for file_node in env['OBJECTS']:
#	print file_node.path



# Ex: easily manage list of files
common_sources = Glob('*.cpp')

# list of main files
target_source_files = ('test-bitvec.cpp',
                       'test-linkedList.cpp',
                       'test-doublyLinkedList.cpp',
                       'test-binarySearchTree.cpp',
                       'test-AVLTree.cpp',
                       'test-sorting.cpp',
                       'test-graph.cpp',
                       'test-binarySearchList.cpp',
                       'test-vector.cpp',
                       'test-btree.cpp',
                       'test-word-counter.cpp',
                       'test-RnBTree.cpp')

# remove target sources
for target_source in target_source_files:
    common_sources.remove(File(target_source))

#for file_node in common_sources:
#	print file_node.path

#common_sources = Split('A.cpp B.cpp')
#common_sources = Split('''A.cpp
#						B.cpp
#						C.cpp''')

# make targets
default_targets = []
for target_source in target_source_files:
    # get rid of filename extension .cpp
    target_name = os.path.splitext(target_source)[0]
    # make a new target
    new_target = env.Program(target=target_name, source=[target_source, common_sources ], CPPPATH=['.'] )
    default_targets.append(new_target)

#env.Command('file.out', 'file.in', "sed 's/a/b/g' < $SOURCE  > $TARGET")
env.Default(default_targets)
