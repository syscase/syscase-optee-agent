# Change Log

## [Unreleased](https://github.com/zaidan/optee_agent/tree/HEAD)

[Full Changelog](https://github.com/zaidan/optee_agent/compare/0.4.0...HEAD)

**Implemented enhancements:**

- Add blacklisting of OPTEE system calls [\#20](https://github.com/zaidan/optee_agent/pull/20) ([zaidan](https://github.com/zaidan))

## [0.4.0](https://github.com/zaidan/optee_agent/tree/0.4.0) (2018-04-16)

[Full Changelog](https://github.com/zaidan/optee_agent/compare/0.3.0...0.4.0)

**Implemented enhancements:**

- Add OPTEE SMC message type [\#19](https://github.com/zaidan/optee_agent/pull/19) ([zaidan](https://github.com/zaidan))
- Add flag SYSCASE\_SMC [\#18](https://github.com/zaidan/optee_agent/pull/18) ([zaidan](https://github.com/zaidan))

## [0.3.0](https://github.com/zaidan/optee_agent/tree/0.3.0) (2018-04-01)

[Full Changelog](https://github.com/zaidan/optee_agent/compare/0.2.0...0.3.0)

**Implemented enhancements:**

- Add SMC call fuzzing [\#17](https://github.com/zaidan/optee_agent/pull/17) ([zaidan](https://github.com/zaidan))
- Add .checkpatch.conf [\#15](https://github.com/zaidan/optee_agent/pull/15) ([zaidan](https://github.com/zaidan))
- Add common.c for SMC call fuzzing [\#14](https://github.com/zaidan/optee_agent/pull/14) ([zaidan](https://github.com/zaidan))

**Fixed bugs:**

- Compiled LKM does not write correct arguments for afl\_call hypercall [\#16](https://github.com/zaidan/optee_agent/issues/16)

## [0.2.0](https://github.com/zaidan/optee_agent/tree/0.2.0) (2018-03-11)

[Full Changelog](https://github.com/zaidan/optee_agent/compare/0.1.0...0.2.0)

**Implemented enhancements:**

- Add CHANGELOG [\#5](https://github.com/zaidan/optee_agent/issues/5)
- Add parsing and basic OPTEE system call fuzzing support in TA [\#13](https://github.com/zaidan/optee_agent/pull/13) ([zaidan](https://github.com/zaidan))
- Refactor Syscase to support parsing in TA [\#11](https://github.com/zaidan/optee_agent/pull/11) ([zaidan](https://github.com/zaidan))
- Add OPTEE system call execution [\#10](https://github.com/zaidan/optee_agent/pull/10) ([zaidan](https://github.com/zaidan))
-  Add Linux and OPTEE mode to switch between tracing targets [\#9](https://github.com/zaidan/optee_agent/pull/9) ([zaidan](https://github.com/zaidan))
- Add support for variable system call argument array length [\#8](https://github.com/zaidan/optee_agent/pull/8) ([zaidan](https://github.com/zaidan))
- Add test case parsing [\#7](https://github.com/zaidan/optee_agent/pull/7) ([zaidan](https://github.com/zaidan))
- Send input string to TA [\#6](https://github.com/zaidan/optee_agent/pull/6) ([zaidan](https://github.com/zaidan))

**Fixed bugs:**

- Fix OPTEE system call arguments [\#12](https://github.com/zaidan/optee_agent/pull/12) ([zaidan](https://github.com/zaidan))

## [0.1.0](https://github.com/zaidan/optee_agent/tree/0.1.0) (2018-02-01)

[Full Changelog](https://github.com/zaidan/optee_agent/compare/0.0.1...0.1.0)

**Implemented enhancements:**

- Add simple test case [\#4](https://github.com/zaidan/optee_agent/pull/4) ([zaidan](https://github.com/zaidan))

## [0.0.1](https://github.com/zaidan/optee_agent/tree/0.0.1) (2018-02-01)

[Full Changelog](https://github.com/zaidan/optee_agent/compare/fd6b1e5cc19ce001044784534c5148244bf7d253...0.0.1)

**Implemented enhancements:**

- Test concurrent [\#3](https://github.com/zaidan/optee_agent/pull/3) ([zaidan](https://github.com/zaidan))

**Merged pull requests:**

- Revert "Test concurrent" [\#2](https://github.com/zaidan/optee_agent/pull/2) ([zaidan](https://github.com/zaidan))
- Test concurrent [\#1](https://github.com/zaidan/optee_agent/pull/1) ([zaidan](https://github.com/zaidan))
