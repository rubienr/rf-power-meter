<!-- Please do not create a pull request without creating an issue first. -->
<!-- You can skip this if you're fixing a typo. -->

<!-- tick off passed steps by [x] -->
<!-- mark failed steps with   [!] -->
<!-- mark steps not run with  [--] -->


**Description**
TODO
<!-- Please include a summary of the change and which issue(s) it fixes (use `closes #XXXX`) -->
<!-- Please include relevant motivation and context. -->
<!-- List any dependencies that are required for this change. -->


**Type of change**
<!-- Please delete options that are not applicable. -->
- cleanup or refactoring (non-breaking change)
- bug fix (non-breaking change which fixes an issue)
- new feature (non-breaking change which adds functionality)
- breaking change (fix or feature that would cause existing functionality to not work as expected)
- this change requires a documentation update


**Tests to verify changes**
<!-- List old and new tests that verifies your changes or delete this section if not applicable. -->
- Test A
- Test B (new test)
- `pio test --environment test_xxx --filter fff` <!-- with `fff` being a filter pattern, example `pio test -e test_arduino -f test_crc` -->
- ...
- Test N


**Checklist**
- [ ] I have installed the pre-commit hook or at least run it manually: `pre-commit install` or `pre-commit run --all-files` <!-- note: `pre-commit install` enables pre-commit hook just for upcoming commits -->
- [ ] my code follows the style guidelines of this project
- [ ] I have performed a self-review of my own code
- [ ] I have commented my code in hard-to-understand areas and public interface
- [ ] I have made corresponding changes to the documentation
- [ ] my changes generate no new warnings
- [ ] I have added tests that prove my fix is effective or that my feature works
- [ ] new and existing unit tests pass locally with my changes
- [ ] I have checked my code and corrected any misspellings


**Testing**
- unit tests:
  - [ ] in GitHub CI: triggered on commit
  - [ ] on your machine: `pio test --environment test_native`
  - [ ] `pio test --environment test_arduino_uno` <!-- test at least with one controller; prefer as many as possible -->
<!-- - [ ] `pio test -e test_ststm32` // if applicable: other devices that have been tested with-->

