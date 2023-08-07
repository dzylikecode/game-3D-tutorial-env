#!/usr/bin/env zx

const path = require('path');

const THISDIR = path.dirname(process.argv[1]);
const SHADERS_DIR = 'shaders/';

const TARGETS = {
  0: { name: 'hlsl', platform: 'windows', vs: 's_3_0', fs: 's_3_0' },
  1: { name: 'hlsl', platform: 'windows', vs: 's_5_0', fs: 's_5_0', cs: 's_5_0' },
  2: { name: 'essl', platform: 'nacl' },
  3: { name: 'essl', platform: 'android' },
  4: { name: 'glsl', platform: 'linux', vs: '120', fs: '120', cs: '430' },
  5: { name: 'metal', platform: 'osx' },
  6: { name: 'pssl', platform: 'orbis' },
  7: { name: 'spirv', platform: 'linux' }
};

const ADDITIONAL_INCLUDES = '';

function getFlags(target, type) {
  const flags = `--platform ${TARGETS[target].platform}`;
  if (type === 'vs') {
    return `${flags} -p ${TARGETS[target].vs} -O 3`;
  } else if (type === 'fs') {
    return `${flags} -p ${TARGETS[target].fs} -O 3`;
  } else if (type === 'cs') {
    return `${flags} -p ${TARGETS[target].cs} -O 1`;
  }
  return flags;
}

function getSources(type) {
  return $`ls ${SHADERS_DIR}${type}_*.sc`.toString().split('\n').filter(Boolean);
}

function getDeps(sources) {
  return sources.map(source => {
    const basename = path.basename(source, '.sc');
    return `${BUILD_INTERMEDIATE_DIR}/${basename}.bin.d`;
  });
}

function getBinaries(sources, type) {
  return sources.map(source => {
    const basename = path.basename(source, '.sc');
    return `${BUILD_INTERMEDIATE_DIR}/${basename}.bin`;
  });
}

function compileShader(source, flags, type) {
  const basename = path.basename(source, '.sc');
  const output = `${BUILD_INTERMEDIATE_DIR}/${basename}.bin`;
  console.log(`[${source}]`);
  $`shaderc ${flags} --type ${type} --depends -o ${output} -f ${source} --disasm`;
  $`cp ${output} ${BUILD_OUTPUT_DIR}/${path.basename(output)}`;
}

function compileShaders(target) {
  const BUILD_OUTPUT_DIR = `./${RUNTIME_DIR}/shaders/${TARGETS[target].name}`;
  const BUILD_INTERMEDIATE_DIR = `${BUILD_DIR}/shaders/${TARGETS[target].name}`;
  const VS_FLAGS = `${getFlags(target, 'vs')} -i ${THISDIR}../src/ ${ADDITIONAL_INCLUDES}`;
  const FS_FLAGS = `${getFlags(target, 'fs')} -i ${THISDIR}../src/ ${ADDITIONAL_INCLUDES}`;
  const CS_FLAGS = `${getFlags(target, 'cs')} -i ${THISDIR}../src/ ${ADDITIONAL_INCLUDES}`;
  const VS_SOURCES = getSources('vs');
  const FS_SOURCES = getSources('fs');
  const CS_SOURCES = getSources('cs');
  const VS_DEPS = getDeps(VS_SOURCES);
  const FS_DEPS = getDeps(FS_SOURCES);
  const CS_DEPS = getDeps(CS_SOURCES);
  const VS_BIN = getBinaries(VS_SOURCES, 'vs');
  const FS_BIN = getBinaries(FS_SOURCES, 'fs');
  const CS_BIN = getBinaries(CS_SOURCES, 'cs');
  const BIN = [...VS_BIN, ...FS_BIN, ...CS_BIN];
  const ASM = [];

  $`mkdir -p ${BUILD_INTERMEDIATE_DIR}`;
  $`mkdir -p ${BUILD_OUTPUT_DIR}`;

  VS_SOURCES.forEach(source => compileShader(source, VS_FLAGS, 'vertex'));
  FS_SOURCES.forEach(source => compileShader(source, FS_FLAGS, 'fragment'));
  CS_SOURCES.forEach(source => compileShader(source, CS_FLAGS, 'compute'));

  console.log(`Target ${path.basename(process.cwd())} / shaders/${TARGETS[target].name}`);
}

if (!process.env.TARGET) {
  console.log('Usage: zx shader.js TARGET=# [clean, all, rebuild]');
  console.log('  TARGET=0 (hlsl  - d3d9  / Windows only!)');
  console.log('  TARGET=1 (hlsl  - d3d11 / Windows only!)');
  console.log('  TARGET=3 (essl  - android)');
  console.log('  TARGET=4 (glsl)');
  console.log('  TARGET=5 (metal)');
  console.log('  TARGET=6 (pssl)');
  console.log('  TARGET=7 (spirv)');
} else {
  const TARGET = parseInt(process.env.TARGET);
  const BUILD_OUTPUT_DIR = `./${RUNTIME_DIR}/shaders/${TARGETS[TARGET].name}`;
  const BUILD_INTERMEDIATE_DIR = `${BUILD_DIR}/shaders/${TARGETS[TARGET].name}`;

  switch (process.argv[2]) {
    case 'all':
      compileShaders(TARGET);
      break;
    case 'clean':
      $`rm -vf ${BUILD_INTERMEDIATE_DIR}/*.bin`;
      break;
    case 'rebuild':
      $`rm -vf ${BUILD_INTERMEDIATE_DIR}/*.bin`;
      compileShaders(TARGET);
      break;
    default:
      console.log(`Usage: zx shader.js TARGET=# [clean, all, rebuild]`);
      break;
  }
}