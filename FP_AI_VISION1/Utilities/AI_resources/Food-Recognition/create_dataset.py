from PIL import Image
import os
import click
import shutil

resize_methods = {'nearest': Image.NEAREST,
                  'bilinear': Image.BILINEAR,
                  'bicubic': Image.BICUBIC,
                  'lanczos': Image.LANCZOS}


@click.command()
@click.argument("path", required=True)
@click.option("--output", default="dataset", help="Name of the output directory")
@click.option("--resize/--no-resize", default=False, help="Resize images ? Disabled by default")
@click.option('--resize-method', default='lanczos', type=click.Choice(['nearest', 'bilinear', 'bicubic', 'lanczos']))
@click.option("--width", default=224, help="Target width if resize is True")
@click.option("--height", default=224, help="Target height if resize is True")
def create_dataset(path, output, resize, resize_method, width, height):
    '''Creates a dataset of PPM images to use in Onboard validation application

    Reads images in any format from a directory and saves them in an ouput directory in Pixmap binary fromat (PPM P6).
    '''
    if not os.path.exists(output):
        os.makedirs(output)
    else:
        valid_input = False
        while not valid_input:
            print('directory {} already exists. Do you want to remove it ? (y/n)'.format(output))
            choice = input()
            if choice == 'y':
                shutil.rmtree(output, ignore_errors=True)
                valid_input = True
            elif choice == 'n':
                print('Cannot continue. Exiting')
                exit(-1)


    # Filter out the directories
    labels = [f for f in os.listdir(path) if os.path.isdir(os.path.join(path, f))]
    print(labels)

    for label in labels:
        label_full_path = os.path.join(path, label)
        for filename in os.listdir(label_full_path):
            file_full_path = os.path.join(label_full_path, filename)
            if os.path.isdir(file_full_path): # Ignore directories
                continue

            filename_no_ext = filename.split('.')[0]
            # in the case of food demo, the classname is not the same as in the Food-101 dataset
            # to match the labels in the code, underscores must be replaced
            # with spaces and words needs to be capitalized
            #label = label.replace('_', ' ').title()
            output_classdir = os.path.join(output, label) #output + '/' + label

            if not os.path.exists(output_classdir):
                os.makedirs(output_classdir)

            img_path = os.path.join(path, label, filename)
            output_file_path = os.path.join(output, label, '{}.ppm'.format(filename_no_ext))
            try:
                im = Image.open(img_path)
                w, h = im.size
                if w == 0 or h == 0:
                    print('Invalid image')
                    continue
                if im.mode != 'RGB':
                    print('Only RGB image are supported')
                    continue

                if resize:
                    im = im.resize((width, height), resample=resize_methods[resize_method])

                im.save(output_file_path)
                print('saving {}'.format(output_file_path))
            except Exception as e:
                print('unable to open {}: {}'.format(img_path, e))


if __name__ == '__main__':
    create_dataset()
