"""Definition of __version__ and __date__ for pyobjcryst.
"""

# module version
__id__ = "$Id$"

# obtain version information
from pkg_resources import get_distribution
__version__ = get_distribution('pyobjcryst').version

# we assume that tag_date was used and __version__ ends in YYYYMMDD
__date__ = __version__[-8:-4] + '-' + \
           __version__[-4:-2] + '-' + __version__[-2:]


# End of file