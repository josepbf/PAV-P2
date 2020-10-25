project('Práctica 2 de PAV - detección de actividad vocal', 'c')

src = ['src/main_vad.c', 'src/vad.c', 'src/pav_analysis.c']
lib = ['-lm', '-lsndfile']
exe = 'vad'

inputDO = files('src/vad.docopt')
progDO = find_program('docopt_c/docopt_c.py')
outputDO = 'vad_docopt.h'

headerDO = custom_target('Construye vad_docopt.h', input: inputDO,
        output: outputDO, command: [progDO, '@INPUT@', '-o', '@OUTPUT@'])

executable(exe, sources: [src, headerDO], link_args: lib)