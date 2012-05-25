#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FLAG_NOISEY 1
#define FLAG_COUNT 2
#define FLAG_BYTE_OFFSET 4
#define FLAG_IGNORE_CASE 8
#define FLAG_INVERT_MATCH 16
#define FLAG_WITH_FILENAME 32

int
bsearch_file_core (FILE * f, off_t fsize,
		   char *buf, size_t sz,
		   const char *needle, const char *fname, int flags)
{

  int cmp;
  int count = 0;
  size_t imin = 0;
  size_t imax = fsize;
  size_t offset = 0;

  while (imax >= imin)
    {
      size_t imid = (imin + imax) >> 1;

      if (fseek (f, imid, SEEK_SET) != 0)
	{
	  return -6;
	}
      if (fgets (buf, sz, f) == NULL)
	{
	  return -7;
	}

      if (imid > 0)
	{
	  offset = imid + strlen (buf);
	  if (fgets (buf, sz, f) == NULL)
	    {
	      return -8;
	    }
	}
      else
	{
	  offset = 0;
	}

      size_t slen1 = strlen (buf);
      if (slen1 > 0 && buf[slen1 - 1] == '\n')
	{
	  buf[slen1 - 1] = '\0';
	}

      if (flags & FLAG_IGNORE_CASE)
	{
	  cmp = strcasecmp (buf, needle);
	}
      else
	{
	  cmp = strcmp (buf, needle);
	}

      if (cmp != 0)
	{
	  if (cmp < 0)
	    {
	      imin = imid + 1;
	    }
	  else
	    {			//if (cmp > 0) {
	      imax = imid - 1;
	    }
	  count += 1;

	  if ((flags & FLAG_NOISEY) && (flags & FLAG_INVERT_MATCH)
	      && !(flags & FLAG_COUNT))
	    {
	      if (flags & FLAG_WITH_FILENAME)
		{
		  printf ("%s:", fname);
		}
	      if (flags & FLAG_BYTE_OFFSET)
		{
		  printf ("%ld:", offset);
		}
	      printf ("%s\n", buf);
	    }
	}
      else
	{
	  if (flags & FLAG_NOISEY)
	    {
	      if (flags & FLAG_WITH_FILENAME)
		{
		  printf ("%s:", fname);
		}

	      if (flags & FLAG_COUNT)
		{
		  printf ("%d\n", (flags & FLAG_INVERT_MATCH) ? count : 1);
		  return 0;
		}
	      if (flags & FLAG_BYTE_OFFSET)
		{
		  printf ("%ld:", offset);
		}
	      printf ("%s\n", buf);
	    }
	  return 0;
	}
    }

  if ((flags & FLAG_NOISEY) && (flags & FLAG_COUNT))
    {
      if (flags & FLAG_WITH_FILENAME)
	{
	  printf ("%s:", fname);
	}
      if (flags & FLAG_COUNT)
	{
	  printf ("%d\n", (flags & FLAG_INVERT_MATCH) ? count : 0);
	}
    }
  return 1;
}

int
bsearch_file (const char *needle, const char *fname, int flags)
{
  struct stat statbuf;
  int ok = stat (fname, &statbuf);
  if (ok != 0)
    {
      return -2;
    }
  // check if regular file
  if (!S_ISREG (statbuf.st_mode))
    {
      return -3;
    }
  // get size of file
  off_t fsize = statbuf.st_size;

  // open
  FILE *f = fopen (fname, "rb");
  if (f == NULL)
    {
      return -4;
    }
  // figure out buffer size
  size_t sz = 2 * strlen (needle);
  if (256 > sz)
    {
      sz = 256;
    }
  char *buf = (char *) malloc (sz);
  if (buf == NULL)
    {
      return -5;
    }

  ok = bsearch_file_core (f, fsize, buf, sz, needle, fname, flags);

  fclose (f);
  free (buf);

  return ok;
}

int
main (int argc, char **argv)
{

  int flags = FLAG_NOISEY;
  int showfiles = 0;

  int c;
  while ((c = getopt (argc, argv, "bciyqhHvVx")) != -1)
    {
      switch (c)
	{
	case 'b':
	  flags |= FLAG_BYTE_OFFSET;
	  break;
	case 'c':
	  flags |= FLAG_COUNT;
	  break;
	case 'y':
	case 'i':
	  flags |= FLAG_IGNORE_CASE;
	  break;
	case 'h':
	  showfiles = 1;
	  break;
	case 'H':
	  showfiles = 2;
	  break;
	case 'q':
	  flags = (flags & (!(FLAG_NOISEY)));
	  break;
	case 'v':
	  flags |= FLAG_INVERT_MATCH;
	  break;
	case 'x':
	  // "Select only those matches that exactly match the whole line."
	  // This is bgrep, so this option is only for compatibily with grep
	  break;
	case 'V':
	  fprintf (stderr, "%s\n", "1");
	  exit (0);
	default:
	  break;
	  // NOP
	}
    }

  if (argc - optind < 2)
    {
      fprintf (stderr, "Usage: %s term file\n\n", argv[0]);
      return -1;
    }

  const char *needle = argv[optind];
  optind += 1;

  if (argc - optind == 1)
    {
      // just one file
      if (showfiles == 2)
	{
	  flags |= FLAG_WITH_FILENAME;
	}
    }
  else
    {
      // more than one file
      if (showfiles != 1)
	{
	  flags |= FLAG_WITH_FILENAME;
	}
    }

  int ok = 0;
  int index = 0;
  for (index = optind; index < argc; ++index)
    {
      ok |= bsearch_file (needle, argv[index], flags);
    }

  if (ok == 0)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}
