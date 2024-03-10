#! bin/usr/env python

import argparse
from IPython.display import HTML
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd


# Create a function to update the plot for each frame
def update(frame, ax, df_time):
    """
    Updates the plot for each frame, where ax is the axes class and df_time is
    the pandas dataframe grouped by time.
    """
    # Clear the plot with each frame
    ax.clear()
    # Get each column for a given time
    sub_df = df_time.get_group(frame)

    ax.plot(sub_df["position"], sub_df["u"], label="u(x)")
    ax.plot(sub_df["position"], sub_df["v"], label="v(x)")
    ax.legend()
    ax.set_ylim(-3, 3)
    ax.set_title(f"Time = {frame}")
    ax.set_xlabel("Position (x)")
    ax.set_ylabel("Concentration")


def main(data, save):
    # Read the textfile
    df = pd.read_csv(data, header=None, names=["time", "position", "u", "v"])
    df_time = df.groupby("time")

    # Define the plot
    fig, ax = plt.subplots()

    # Create the animation
    anim = FuncAnimation(
        fig,
        update,
        fargs=(ax, df_time),
        frames=df["time"].unique(),
        interval=20,
        repeat=True,
    )
    # Display the plot
    plt.show()

    # Save the animation to an HTML file
    if save:
        html_output = f"{data}_anim.html"
        anim.save(html_output, writer="html")
        HTML(html_output)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("data", help="data file")
    parser.add_argument("-s", "--save", action="store_true", help="save animation")
    args = parser.parse_args()

    main(args.data, args.save)
