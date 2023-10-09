import datetime
import dateutil.tz

from flask import Blueprint, render_template


from . import model

bp = Blueprint("main", __name__)


@bp.route("/")
def index():
    user = model.User(1, "mary@example.com", "mary")
    posts = [
        model.Message(
            1, user, "Test post", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
        model.Message(
            2, user, "Another post", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
        model.Message(
            3, user, "Another post test", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
    ]
    return render_template("main/index.html", posts=posts)
@bp.route('/profile')
def profile():
    user = model.User(1, "tom@example.com", "tom")
    posts = [
        model.Message(
            1, user, "Test post", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
        model.Message(
            2, user, "Another post", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
        model.Message(
            3, user, "Another post test", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
    ]
    return render_template('main/profile.html', posts=posts)
@bp.route('/posts')
def posts():
    user = model.User(1, "jason@example.com", "jason")
    post = model.Message(
            1, user, "Main Post", datetime.datetime.now(dateutil.tz.tzlocal())
        )
    replies = [
        model.Message(
            1, user, "reply post", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
        model.Message(
            2, user, "reply post2", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
        model.Message(
            3, user, "Another post test", datetime.datetime.now(dateutil.tz.tzlocal())
        ),
    ]
    return render_template('main/posts.html', posts=replies, post=post)
