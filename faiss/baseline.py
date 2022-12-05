import pandas as pd


def main():
    train_filename = r'C:\Users\Sergey\Documents\GitHub\gpu-clusterize\faiss\train.csv'
    train = pd.read_csv(train_filename)
    color_names = train.columns[1:]

    print(train.head(n=10))
    # computing the most popular number of samples for each color
    color_blocks = {}
    for color in color_names:
        color_blocks[color] = train[color].value_counts().index[1]
        print(f'{color}: {color_blocks[color]}')

    submit_filename = r'C:\Users\Sergey\Documents\GitHub\gpu-clusterize\faiss\tsubmit.csv'
    submit = pd.read_csv(submit_filename)

    for idx, dialog in enumerate(submit['dialog']):
        # if color is in the dialog, then set the number of blocks using color_blocks
        for color in color_names:
            submit.loc[idx, color] = color_blocks[color] if dialog.find(color) != -1 else 0

    # save the results to csv file
    submit.to_csv(submit_filename, index=False)


if __name__ == '__main__':
    main()
