# Basic Classes For Slam

***Some basic classes for SLAM C++ developers. If you like it, star this project please. Thank you!***

----------------------------------

## Classes

|               All classes              |
| :--------: | :----------: | :--------: |
| Exception  |  YamlReader  |  Configure |

### Exception

This class inherits *std::exception*, but provides richer function.

*SLAM::Exception* can be constructed with none parameter or some informations about error.
Differently from others, it can record where the error occured and which third party library raise the exception.

### YamlReader

*SLAM::Basic::YamlReader* is a implement based on third party library.
It unifies some interfaces and can interpret bool, long, cv::Mat, Eigen::Matrix and so on.
It should be noted that *cv::FileStorage* could't recognize *true* and *false* and *YAML::Node* could't read *cv::Mat*.

In particular, it provides '*Read*' and '*ReadSafe*' method.
'*Read*' accepts a *name* which means the key name and return the value corresponding to *name*.
The value also can be returned by referencing parameters.
Compared to '*Read*', '*ReadSafe*' accept additional a default value that will be set if fail to read.

And it also provides some static function to read value and some immature writing interfaces(*YamlWriter*, *YamlIO*).

### Configure

*SLAM::Configure* is used for reading setup files.
It is designed to be called as a single instance, through *I* or *GetInstance* static method.
Of course, you can reform it if it doesn't match your requirements.
And *YamlReader* is responsible for parsing and reading functions.

----------------------------------

## Dependencies

It requires *OpenCV* or *Yaml-cpp* to read files.

----------------------------------

## Trouble

If the classes don't work as well as your wishes,

* check the previous issues
* add a new issues.
