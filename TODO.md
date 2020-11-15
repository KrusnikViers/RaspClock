### Deployment

* [ ] Check remote version and compare with current one
* [ ] Figure out, how to self-update

### Functional

* [ ] AirQuality data
* [ ] Time Zone based on geodata
* [ ] Display precise time and date
* [ ] Weather based on geodata

### Quality
* [ ] Create tests to cover most critical parts


```
- git remote add gh https://${TRAVIS_REPO_SLUG%/*}:${GHUB_TOKEN}@github.com/${TRAVIS_REPO_SLUG}.git
- git push -f gh latest-ci
- git remote remove gh
```
