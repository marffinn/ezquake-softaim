# GitHub Pages Deployment Guide

## Setting up GitHub Pages for ezQuake SoftAim

### Option 1: Using docs folder (Recommended)

1. **Copy the docs folder** to your main branch:
   ```bash
   # Copy the entire docs folder to your repository root
   cp -r docs/ /path/to/your/ezquake-softaim-repo/
   ```

2. **Enable GitHub Pages**:
   - Go to your repository settings on GitHub
   - Scroll down to "Pages" section
   - Under "Source", select "Deploy from a branch"
   - Choose "main" branch and "/docs" folder
   - Click "Save"

3. **Your site will be available at**:
   `https://marffinn.github.io/ezquake-softaim/`

### Option 2: Using gh-pages branch

1. **Create and switch to gh-pages branch**:
   ```bash
   git checkout --orphan gh-pages
   git rm -rf .
   ```

2. **Copy documentation files**:
   ```bash
   cp docs/index.html .
   cp docs/README.md .
   cp docs/_config.yml .
   cp docs/download-tracker.js .
   ```

3. **Commit and push**:
   ```bash
   git add .
   git commit -m "Initial GitHub Pages setup"
   git push origin gh-pages
   ```

4. **Enable GitHub Pages**:
   - Go to repository settings
   - Under "Pages", select "gh-pages" branch
   - Choose "/ (root)" folder

## Download Counter Setup

The download counters are automatically configured with these badges:

### Total Downloads
```markdown
![Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/total?style=for-the-badge&logo=github&label=Total%20Downloads&color=blue)
```

### Latest Release Downloads
```markdown
![Latest Release Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/latest/total?style=for-the-badge&logo=download&label=Latest%20Release&color=green)
```

### Specific File Downloads
If you want to track a specific file (like ezquake.exe):
```markdown
![EXE Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/latest/ezquake.exe?style=for-the-badge&label=ezquake.exe&color=orange)
```

## Features Included

✅ **Download Counter Badges** - Automatic tracking via GitHub API  
✅ **Responsive Design** - Works on desktop and mobile  
✅ **Feature Documentation** - Complete cl_rDrive and SoftAim docs  
✅ **Direct Download Links** - Links to latest release  
✅ **SEO Optimized** - Proper meta tags and structure  
✅ **Analytics Ready** - Google Analytics integration support  

## Customization

### Updating Download Links
Edit the download URLs in `index.html` and `README.md` to match your release naming:
```html
<a href="https://github.com/marffinn/ezquake-softaim/releases/latest/download/ezquake.exe">
```

### Changing Colors/Styling
Modify the CSS in `index.html` or add a separate stylesheet.

### Adding Analytics
Add Google Analytics or other tracking code to `index.html` in the `<head>` section.

## Maintenance

The download counters update automatically, but you should:
1. Update documentation when adding new features
2. Update version numbers and release notes
3. Test download links after each release
4. Monitor the GitHub Pages build status

## Troubleshooting

**Pages not updating?**
- Check the Actions tab for build errors
- Ensure the branch/folder settings are correct
- Clear browser cache

**Download counters not working?**
- Verify repository name and owner in badge URLs
- Check if repository is public
- GitHub API has rate limits (usually not an issue)

**Links broken?**
- Verify release exists and file names match
- Check if repository name changed