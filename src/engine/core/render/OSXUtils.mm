#import <Cocoa/Cocoa.h>
#include "OSXUtils.h"

size_t getNSViewFromNSWindow(size_t windowHandle)
{
  NSWindow* window = reinterpret_cast<NSWindow*>(windowHandle);
  NSView* view = [window contentView];
  return reinterpret_cast<size_t>(view);
}