# Contributing Guidelines

So you want to contribute to Splash Text?  That's great to hear!  This document describes the guidelines for the four methods of contributing:  contributing new splashes, filing issues, and opening pull requests.

## New Splashes

Contributing new splashes is done by opening a pull request (see below) modifying the appropriate splash document.  You can contributed as many splashes as you like in a single pull request.  See the manpage for how the splash files work.  Note that for the presupplied splashes, the following types of content are completely prohibited:

- Spoilers of any size for any piece of media.
- Content that seems to be intended specifically to offend.
- Text that is liable to break things (such as invalid UTF-8 sequences).
- Any content that is illegal, unless such laws are immoral.
- Anything that's overly specific to a particular application—see the manpage for how you can have those exclusively be used in your own application.

## Issues

_**IF YOU HAVE FOUND A SECURITY PROBLEM IN THE SOFTWARE, DO NOT OPEN AN ISSUE FOR IT.  THIS WILL MAKE IT PUBLIC, WHICH IS REALLY BAD FOR SECURITY ISSUES.  SEE THE DOCUMENTATION IN [SECURITY.md](./SECURITY.md) INSTEAD.**_

This project does not make a distinction between bug reports and feature requests—it only has issue reports.  The reasoning behind this is a bit complicated, and i (Blue-Maned\_Hawk, the guy writing this) intend to write an essay about it eventually, but basically, if an addition to the software does not solve a problem with the software, it is bloat, and therefore not worth adding; therefore, we only keep track of the _problems_ in the software.  _If you're disappointed because you wanted to open a feature request, don't worry_—after all, you wanted to open it for a reason, so you can almost surely reword it as being a problem with the software instead.

This project uses an issue template.  Just make sure your issue hasn't been reported already, follow the template, and you should be fine.

## Pull Requests

As explained above, this project considers things to only be worth adding if they solve some sort of problem with the software, regardless of whether it would traditionally be considered a "bug".  _Again, don't be disappointed if you wanted to make a different kind of pull request_—you must have wanted to open the request for a reason, and you can almost surely explain how it fixes a problem with the software.

If you desire to contribute code to the project, please open a pull request and describe the problem which your contribution seeks to solve and how it solves it.  Before beginning work on a pull request, you should check that nobody else is already solving the problem, and you may want to open a draft pull request to get feedback on your idea if it's exceptionally large (or otherwise will require significant effore on your part) so that you don't spend time on something that ends up getting rejected.
