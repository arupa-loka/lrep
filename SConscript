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
# remove target sources
common_sources.remove(File('test-bitvec.cpp'))
common_sources.remove(File('test-linkedList.cpp'))
common_sources.remove(File('test-doublyLinkedList.cpp'))
common_sources.remove(File('test-binarySearchTree.cpp'))
common_sources.remove(File('test-AVLTree.cpp'))
common_sources.remove(File('test-sorting.cpp'))
common_sources.remove(File('test-graph.cpp'))
common_sources.remove(File('test-binarySearchList.cpp'))
common_sources.remove(File('test-vector.cpp'))
common_sources.remove(File('test-btree.cpp'))
common_sources.remove(File('test-word-counter.cpp'))

#for file_node in common_sources:
#	print file_node.path

#common_sources = Split('A.cpp B.cpp')
#common_sources = Split('''A.cpp
#						B.cpp
#						C.cpp''')

target02 = env.Program(target='test-bitvec', source=['test-bitvec.cpp', common_sources ], CPPPATH=['.'] )
target03 = env.Program(target='test-linkedList', source=['test-linkedList.cpp', common_sources ], CPPPATH=['.'] )
target04 = env.Program(target='test-doublyLinkedList', source=['test-doublyLinkedList.cpp', common_sources ], CPPPATH=['.'] )
target05 = env.Program(target='test-binarySearchTree', source=['test-binarySearchTree.cpp', common_sources ], CPPPATH=['.'] )
target06 = env.Program(target='test-AVLTree', source=['test-AVLTree.cpp', common_sources ], CPPPATH=['.'] )
target07 = env.Program(target='test-sorting', source=['test-sorting.cpp', common_sources ], CPPPATH=['.'] )
target08 = env.Program(target='test-graph', source=['test-graph.cpp', common_sources ], CPPPATH=['.'] )
target09 = env.Program(target='test-binarySearchList', source=['test-binarySearchList.cpp', common_sources ], CPPPATH=['.'] )
target10 = env.Program(target='test-vector', source=['test-vector.cpp', common_sources ], CPPPATH=['.'] )
target11 = env.Program(target='test-btree', source=['test-btree.cpp', common_sources ], CPPPATH=['.'] )
target12 = env.Program(target='test-word-counter', source=['test-word-counter.cpp', common_sources ], CPPPATH=['.'] )

#env.Command('file.out', 'file.in', "sed 's/a/b/g' < $SOURCE  > $TARGET")

env.Default(target02, target03, target04, target05, target06, target07, target08, target09, target10, target11, target12)



