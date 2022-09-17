# Contributing Guidelines

So you want to contribute to Splash Text?  That's great to hear!  This document describes the guidelines for the four methods of contributing:  contributing new splashes, filing issues, and opening pull requests.

## New Splashes

Contributing new splashes is done by opening a pull request modifying the appropriate splash document.  You can contributed as many splashes as you like in a single pull request.  Splash text documents are named in the form "`Unique Name.abc.x.splash.txt`", where `Unique Name` is a unique name, `.abc` are optional descriptions of the restricted content (see below) found within the document, `.x` is a character describing the contexts in which the splashes contained within make sense (see below), and `.splash.txt` is always the text `.splash.txt`.

The restricted content characters are `x` for sexual content, `g` for graphic content, and `s` for heavy content.  In addition, the character `h` is added if the content within the splash file is (intended to be) humorous.

The context characters are `l` for logs, `c` for crashes, `p` for pseudopromotional statements, `s` for subtitles, `r` for ominous messages, `q` for quotes, and `x` for miscellaneous stuff.

The following types of content are completely prohibited:

	- Spoilers of any size for any piece of media.
	- Content that seems to be intended specifically to offend.
	- Text that is liable to break things (such as invalid UTF-8).
	- Any splash that is more than a kilobyte long, including the newline.
	- Any content that is illegal, unless such laws are immoral.
	- Anything that's overly specific to a particular application.

## Issues

_**IF YOU HAVE FOUND A SECURITY PROBLEM IN THE SOFTWARE, DO NOT OPEN AN ISSUE FOR IT.  THIS WILL MAKE IT PUBLIC.  PLEASE SEE THE DOCUMENTATION IN [SECURITY.md](./SECURITY.md) INSTEAD.**_

This project does not make a distinction between bug reports and feature requests.  It only has issue reports—if an addition to the software does not solve a problem with the software, it is bloat and should not be added, so keeping track of what isn't a problem isn't worth the time.  If you're disappointed because you wanted to open a feature request, don't worry—after all, you wanted to open it for a reason, so you can almost surely reword it as being a problem with the software instead.

This project uses an issue template.  Just follow that, and you should be fine.

## Pull Requests

If you desire to contribute code to the project, please open a pull request and describe the problem which your contribution seeks to solve.  You should also include a lone line with the content `Category: `, followed by the tag for the category to which your pull request pertains.  (This is for the sake of automatic labeling—you don't need to do anything for the other ones.)
