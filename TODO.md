# TODO

---

- [ ] Clean up ('legacy') tags
- [ ] Windows build?
- [ ] openMP build on macOS
- [ ] Code coverage
- [ ] Clean up build scripts (makeall.sh, removebuild.sh)
- [ ] rm TODO.md
- [ ] Update README.md & PULL_REQUEST_TEMPLATE.md
- [ ] Discuss license with OS
- [ ] Update Doxyfile.in
- [ ] Fix GitHub files
- [ ] rm AUTHORS
- [ ] rm .travis.yml
- [ ] Everybody use clang-format (git hooks?)
- [ ] Update util/
- [ ] Move to C++17
  - [ ] ghc:: -> std::
  - [ ] string_view
  - [ ] boost::optional -> std::optional
  - [ ] genie::util::make_unique -> std::make_unique
- [ ] mv test/*.sh to elsewhere
- [ ] Include contents of doc/ in DEVELOPMENT.md
- [ ] Include *.md files in Doxygen
- [ ] rm GABAC configs (data/gabac/)
- [ ] Set up conformance scripts
- [ ] rm src/misc/
- [ ] Rename src/genie/module/
- [ ] Better test files
- [ ] Clean up data/misc
- [ ] Delete web page
- [ ] Update issue templates
- [ ] Clean up ci/
- [ ] Verify exception handling
- [ ] Add GENIE_DIE macro
- [ ] Add shortcut U/I classification
- [ ] rm src/apps/encapsulator
- [ ] rm classifier-bypass
- [ ] Implement C++ Core Guideline C.21 ("If you define or =delete any default operation, define or =delete them all")
- Use clang-format
- [ ] Clean up collaborators on GitHub
- [ ] Remove GABAC optimization stuff
- [ ] Update MITOGEN page
- [ ] Add boilerplate to all source code files
- [ ] Style
  - [ ] Sentences should start with an uppercase letter and end with a full stop.
  - [ ] Statements should start with an uppercase letter and not end with a full stop.
- [ ] Clean up exceptions (use only a simple class derived from std::exception - without all the special things)
- [ ] Implement a unified logging solution
- [ ] Check for correct usage of assertions versus exceptions
- [ ] Make destructors virtual in all classes that something is being derived from (use override in derived classes)
- [ ] Unify I/O
  - [ ] Bitstream I/O should be done using HM's bitstream classes adapted for 64 bits
  - [ ] "Ordinary" file I/O (for e.g. SPRING's temporary files) should be done using the C++ standard library
- [ ] Remove Python interfaces
- [ ] In src/libs/gabac/binary-arithmetic-decoder.cc:112 4 bytes must be skipped. This would not be necessary if bitInputStream and binaryArithmeticDecoder would be using references to the same underlying bitstream.
- [ ] Announce use of GitHub issues
- [ ] Fix clang-tidy warnings
- [ ] Fix compilation warnings
- [ ] Fix Doxygen warnings
- [ ] Clean repository with: https://rtyley.github.io/bfg-repo-cleaner/
- [ ] Check conformance to doc/development-guidelines.md
- [ ] Check everything with Valgrind
- [ ] Resolve FIXMEs and TODOs
- [ ] Update README.md
- [ ] Check LICENSE (check that it is compatible to SPRING's license)
